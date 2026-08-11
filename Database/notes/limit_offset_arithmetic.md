# Why `LIMIT 1 OFFSET N-1` fails in MySQL

Found while solving
[177. Nth Highest Salary](../leetcode/177_Nth_Highest_Salary/README.md).

## The rule

`LIMIT 1 OFFSET N-1` fails. The reason is **not** "SQL forbids arithmetic". The
reason is much narrower:

> MySQL's **parser** accepts only an integer literal or a prepared-statement
> placeholder after `LIMIT` and `OFFSET`. Nothing else. No expression, no
> column, no function call.

That is a rule about **one clause only**. `WHERE` is a normal expression
context. It accepts `N - 1`, `salary * 2`, `COUNT(...)`, anything.

```sql
LIMIT 1 OFFSET N-1        -- error: the LIMIT clause is special
WHERE  N-1 = something    -- fine: WHERE takes any expression
```

So `SET N = N - 1;` is **not** a general workaround for SQL arithmetic. It is a
workaround for that one parser restriction.

## The three escapes

**1. Compute the value first, then use the plain variable.**

```sql
CREATE FUNCTION getNthHighestSalary(N INT) RETURNS INT
BEGIN
  SET N = N - 1;
  RETURN (
      SELECT DISTINCT salary FROM Employee
      ORDER BY salary DESC
      LIMIT 1 OFFSET N
  );
END
```

**2. Use no `LIMIT` at all.** If the clause is absent, the restriction never
applies, and `N - 1` is legal where it stands. This is the correlated subquery
form:

```sql
SELECT DISTINCT e.salary
FROM Employee e
WHERE N - 1 = (
    SELECT COUNT(DISTINCT e2.salary)
    FROM Employee e2
    WHERE e2.salary > e.salary
);
```

**3. Use a window function.** `DENSE_RANK()` gives the rank as a column, and you
compare it in `WHERE`, which takes any expression. See
[the DENSE_RANK note](dense_rank.md).

```sql
... WHERE rnk = N
```

## Two more facts about the same clause

- **A prepared statement accepts `?`.** This is legal, because a placeholder is
  not an expression:
  ```sql
  PREPARE stmt FROM 'SELECT salary FROM Employee ORDER BY salary DESC LIMIT 1 OFFSET ?';
  ```
- **To skip rows with no upper bound**, MySQL still needs a `LIMIT` beside the
  `OFFSET`. Use the largest `BIGINT`:
  ```sql
  LIMIT 18446744073709551615 OFFSET 1
  ```

## This is a MySQL rule, not a SQL rule

| Database | Expression in `LIMIT` / `OFFSET`? |
|---|---|
| MySQL | **No.** Literal or `?` only |
| PostgreSQL | Yes. Any expression |
| SQLite | Yes. Any expression |
| SQL Server | Uses `OFFSET n ROWS FETCH NEXT m ROWS ONLY`; accepts expressions |

So the same query that fails on MySQL can run on PostgreSQL. Do not carry the
`SET N = N - 1` habit to another database and assume it was ever necessary
there.

## Watch out

`OFFSET` cannot be negative. If a caller passes `N = 0`, then `N - 1 = -1` and
the query raises an error. The `DENSE_RANK` form returns `NULL` instead, because
ranks start at 1 and nothing matches.
