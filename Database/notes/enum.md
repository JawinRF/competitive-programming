# `ENUM` — a column with a fixed list of values

Seen in the schema of many LeetCode table problems, for example:

> `activity_type` is an ENUM (category) of type `('start', 'end')`.

## What `ENUM` is

`ENUM` is a string column type with a **closed list** of allowed values. You give
the list when you create the column. MySQL rejects every other value.

MySQL does not store the text in each row. It stores a small **integer index**
into the list. The index starts at 1.

| Stored value | Index |
|---|---|
| `''` (invalid, non-strict mode only) | 0 |
| `'start'` | 1 |
| `'end'` | 2 |
| `NULL` | `NULL` |

So the column costs 1 byte for up to 255 members, and 2 bytes above that. A
`VARCHAR` would cost the length of the text in every row.

## Declare it

```sql
CREATE TABLE Activity (
    user_id       INT,
    session_id    INT,
    activity_date DATE,
    activity_type ENUM('start', 'end') NOT NULL
);
```

The list order is not decoration. It fixes the index of each member, and the
index controls sorting. See the trap below.

To see the list of an existing column:

```sql
SHOW COLUMNS FROM Activity LIKE 'activity_type';
```

## Use it

Read and write the value as a plain string. Nothing special is needed:

```sql
INSERT INTO Activity VALUES (1, 1, '2026-08-13', 'start');

SELECT user_id
FROM Activity
WHERE activity_type = 'start';
```

Conditional counts are the common pattern in these problems:

```sql
SELECT session_id,
       SUM(activity_type = 'start') AS starts,
       SUM(activity_type = 'end')   AS ends
FROM Activity
GROUP BY session_id;
```

For a LeetCode solution, that is the whole story. Treat the column as text.

## The trap — `ORDER BY` uses the index, not the alphabet

```sql
SELECT DISTINCT activity_type FROM Activity ORDER BY activity_type;
```

This returns `start`, then `end`. It does **not** return `end`, then `start`.
The sort compares 1 against 2, not the letters.

This is a feature when the list is already in a natural order:

```sql
ENUM('low', 'medium', 'high')   -- ORDER BY sorts by severity, for free
```

It is a bug when you expect alphabetical order. To force text order:

```sql
ORDER BY CAST(activity_type AS CHAR)
```

`MIN()`, `MAX()` and range comparisons follow the same index rule.
`MIN(activity_type)` on this table gives `'start'`.

## Numbers and enums do not mix well

The index leaks into normal comparisons:

```sql
WHERE activity_type = 1        -- matches 'start'   (compares the index)
WHERE activity_type = '1'      -- matches a member NAMED '1'; here, nothing
SELECT activity_type + 0       -- returns 1 or 2
```

`INSERT ... VALUES (1)` stores `'start'`. Do not write queries this way. The
index is an implementation detail, and it changes when somebody edits the list.

A list of digit strings, such as `ENUM('1', '2', '3')`, is the worst case. There,
`= 1` and `= '1'` mean two different rows. Avoid it.

## Change the list

This is the main weakness of `ENUM`. A new value needs a table change:

```sql
ALTER TABLE Activity
    MODIFY activity_type ENUM('start', 'end', 'pause') NOT NULL;
```

**Add new members at the end.** Then every existing index keeps its meaning. If
you insert `'pause'` in the middle, the index of `'end'` moves from 2 to 3, and
MySQL must rewrite the column in every row.

If the list changes often, or if the values need extra fields, use a lookup
table and a foreign key instead:

```sql
CREATE TABLE ActivityType (id INT PRIMARY KEY, name VARCHAR(20));
```

## Invalid values

| Mode | `INSERT ... VALUES ('paused')` |
|---|---|
| Strict (the default since MySQL 5.7) | Error. The row is rejected |
| Non-strict | Warning. MySQL stores `''` with index 0 |

That index-0 empty string is how you find damaged data after a bad load:

```sql
SELECT * FROM Activity WHERE activity_type = 0;
```

A member name is trimmed of trailing spaces, and the comparison follows the
column collation. With the usual `..._ci` collation, `'START'` matches `'start'`.

## This is a MySQL type, not a SQL type

| Database | Equivalent |
|---|---|
| MySQL | `ENUM('start', 'end')` |
| PostgreSQL | `CREATE TYPE activity AS ENUM ('start', 'end');` then use the type |
| SQL Server | none. Use `CHECK (activity_type IN ('start', 'end'))` |
| SQLite | none. Use the same `CHECK` constraint |

The `CHECK` form is portable and easy to change. It costs the full text in each
row and gives no free ordering.

## Watch out

- `ORDER BY` follows the list order. Read the `CREATE TABLE` before you trust a
  sort.
- Never compare an enum against a number.
- A new value needs `ALTER TABLE`. Append it; do not insert it in the middle.
- A limit of 65,535 members exists. It is never the real limit — a list that
  long belongs in a table.

---

# Learned — two `SELECT`s split by `;` give two answers, not one

The natural first attempt at "pair each start with its end" is two queries:

```sql
SELECT timestamp AS start_time
FROM Activity
WHERE activity_type = 'start'
GROUP BY process_id;

SELECT timestamp AS end_time
FROM Activity
WHERE activity_type = 'end'
GROUP BY process_id;
```

The `;` is legal. Two statements in one script are fine. The mistake is the
expectation.

## What `;` really does

`;` is a **statement terminator**. It says "this statement is complete; run it."
It does not join anything.

The server therefore does this:

1. Run statement 1. Send the whole result to the client. **Forget it.**
2. Run statement 2. Send the whole result to the client.

You get **two result sets**. A judge, or a `RETURN`, reads only one.

## `start_time` does not survive the `;`

An alias is not a variable. It names a column **inside one statement**, and it
dies with that statement. After the `;` there is no `start_time` to reference,
so no later query can subtract it from `end_time`.

The rule: **to combine two things, they must live in one statement.**

## The three ways to stay in one statement

**1. Conditional aggregation.** One pass, both values as columns. This is the
shortest form, and it fits `ENUM` well:

```sql
SELECT process_id,
       MIN(CASE WHEN activity_type = 'end'   THEN timestamp END)
     - MIN(CASE WHEN activity_type = 'start' THEN timestamp END) AS duration
FROM Activity
GROUP BY process_id;
```

`CASE` gives `NULL` on the rows of the other type, and `MIN` ignores `NULL`.

**2. Self-join.** Read the table twice, under two names, and match the keys:

```sql
SELECT s.process_id, e.timestamp - s.timestamp AS duration
FROM Activity s
JOIN Activity e ON s.process_id = e.process_id
WHERE s.activity_type = 'start'
  AND e.activity_type = 'end';
```

**3. CTEs.** Keep the two queries you already wrote, but name them and join
them. Note the commas — there is **no `;` until the end**:

```sql
WITH starts AS (
    SELECT process_id, timestamp AS start_time
    FROM Activity WHERE activity_type = 'start'
),
ends AS (
    SELECT process_id, timestamp AS end_time
    FROM Activity WHERE activity_type = 'end'
)
SELECT s.process_id, e.end_time - s.start_time AS duration
FROM starts s
JOIN ends e ON s.process_id = e.process_id;
```

Form 3 is the direct repair of the broken code: replace the first `;` with a
`WITH` wrapper and a comma.

## The other bug in that code

```sql
SELECT timestamp AS start_time ... GROUP BY process_id;
```

`GROUP BY process_id` makes one row per process, but `timestamp` is bare — it is
not in the `GROUP BY` and it has no aggregate. With `ONLY_FULL_GROUP_BY` on (the
MySQL 8 default), this is an error. Without it, MySQL picks an arbitrary row.

Either aggregate the column, or group by it:

```sql
SELECT process_id, MIN(timestamp) AS start_time ...
```

## Watch out

- Many clients and judges run **one statement only**. A stray `;` in the middle
  can raise a syntax error, not just an unused result.
- `;` inside a stored procedure or function body is fine — the body is one
  statement, and `DELIMITER` protects it.
- Two result sets are never a solution. If the answer needs both, one statement
  must produce both.

---

# Learned — `Operand should contain 1 column(s)`

MySQL error **1241**. It means:

> A subquery gave back **more than one column**, in a place where SQL accepts
> exactly one value.

The count is about **columns**, not rows. That difference is the whole error.

## The usual causes

```sql
-- 1. SELECT * in a scalar position. * is every column, not one column.
WHERE salary = (SELECT * FROM Employee ORDER BY salary DESC LIMIT 1)

-- 2. Two columns where one value fits.
WHERE id = (SELECT id, name FROM Employee LIMIT 1)

-- 3. IN with a two-column subquery, against a single column.
WHERE id IN (SELECT id, name FROM Employee)

-- 4. A wide subquery in the SELECT list.
SELECT name, (SELECT id, salary FROM Employee LIMIT 1) FROM Department
```

Cause 1 is the common one. `SELECT *` is safe inside `FROM` and inside `EXISTS`,
and it fails everywhere a single value is expected.

## The fix — name the one column you want

```sql
WHERE salary = (SELECT salary FROM Employee ORDER BY salary DESC LIMIT 1)
WHERE id     IN (SELECT id FROM Employee)
```

If you truly need to match two columns at once, MySQL has a **row
constructor**. The left side must then have the same width:

```sql
WHERE (id, name) IN (SELECT id, name FROM Employee)   -- legal: 2 against 2
```

If you need columns **from** the other table in your output, a subquery is the
wrong tool. Use a join.

## What each position accepts

| Position | Columns | Rows |
|---|---|---|
| `= ( ... )`, `> ( ... )`, or the `SELECT` list | exactly **1** | 0 or 1 |
| `IN ( ... )` | **1**, or the width of the row constructor | any |
| `EXISTS ( ... )` | any. `SELECT *` is normal here | any |
| `FROM ( ... ) AS t` | any | any |

Read the table this way: the narrower the position, the stricter the shape.

## The sister error — 1242

| Error | Meaning | Fix |
|---|---|---|
| **1241** `Operand should contain 1 column(s)` | too many **columns** | select one column |
| **1242** `Subquery returns more than 1 row` | too many **rows** | add `LIMIT 1`, an aggregate, or change `=` to `IN` |

Both come from the same habit — a subquery used as a value while it returns a
table. Check the column count first, then the row count.

## Watch out

- `SELECT *` in a scalar subquery is the number-one source of 1241. Search for
  it first.
- The error appears at **parse time** for the shape, so it fires even when the
  table is empty.
- A `SELECT *` on a one-column table does work. It then breaks later, when
  somebody adds a column. Always name the column.
