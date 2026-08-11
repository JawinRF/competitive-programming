# `LEAD()` and `LAG()` — read another row from this row

Found while solving
[180. Consecutive Numbers](../leetcode/180_Consecutive_Numbers/README.md).

## What `LEAD()` is

`LEAD` is a window function. It answers one question:

> What is the value of this column, **n rows further down**?

It does not move rows and it does not filter. It **adds a column** to each row,
holding a value copied from a later row.

```sql
LEAD(num, 2) OVER (ORDER BY id)
```

Read it right to left:

1. `OVER (ORDER BY id)` — sort the rows by `id`. This defines what "later"
   means.
2. `LEAD(num, 2)` — from each row, look 2 rows down that order, and take `num`.

## Watch it run

```sql
SELECT id, num,
       LEAD(num, 1) OVER (ORDER BY id) AS next1,
       LEAD(num, 2) OVER (ORDER BY id) AS next2
FROM Logs;
```

On the `Logs` table of problem 180:

| id | num | `next1` | `next2` | `num = next1 = next2`? |
|---|---|---|---|---|
| 1 | 1 | 1 | 1 | **yes** |
| 2 | 1 | 1 | 2 | no |
| 3 | 1 | 2 | 1 | no |
| 4 | 2 | 1 | 2 | no |
| 5 | 1 | 2 | 2 | no |
| 6 | 2 | 2 | `NULL` | no |
| 7 | 2 | `NULL` | `NULL` | no |

Every row now carries its own next two values, side by side. The
three-in-a-row test becomes one plain comparison:

```sql
WHERE num = next1 AND num = next2
```

Row 1 passes. The answer is `1`.

Note what happened: two correlated subqueries became two columns. Same
information, gathered in one sorted pass in place of two lookups per row.

## The tail is `NULL`, and that is correct

Row 7 has no row after it, so `LEAD` gives `NULL`. Then `num = NULL` is `NULL`,
not `TRUE`, and the row drops by itself. You need no extra filter.

A third argument replaces `NULL` with a value of your choice:

```sql
LEAD(num, 1, 'none') OVER (ORDER BY id)
```

## `LAG` is the mirror

| Function | Direction |
|---|---|
| `LEAD(num, 2)` | 2 rows **down** |
| `LAG(num, 2)` | 2 rows **up** |

Nothing else differs. `LAG(num, 1) = num` finds a repeat of the previous row.

Both default to an offset of 1: `LEAD(num)` is `LEAD(num, 1)`.

## `id + 1` and `LEAD` are not the same thing

This is the reason `LEAD` beats id arithmetic:

| | `P.id = L.id + 1` | `LEAD(num, 1) OVER (ORDER BY id)` |
|---|---|---|
| Means | the row whose **id equals** 6 | the row that comes **next in order** |
| After a `DELETE` leaves a hole | finds nothing → `NULL` → misses a real run | steps over the hole, still correct |

`LEAD` counts **positions**. The `id + 1` form counts **id values**. They agree
only while the ids stay contiguous. Autoincrement guarantees increasing ids, not
contiguous ids.

## The trap — the same one as `DENSE_RANK`

You cannot put `LEAD` in `WHERE`:

```sql
-- ERROR
SELECT num FROM Logs
WHERE num = LEAD(num, 1) OVER (ORDER BY id);
```

`WHERE` runs **before** window functions, so the column does not exist yet.
Compute it in a subquery or CTE, then filter outside:

```sql
SELECT DISTINCT num AS ConsecutiveNums
FROM (
    SELECT num,
           LEAD(num, 1) OVER (ORDER BY id) AS next1,
           LEAD(num, 2) OVER (ORDER BY id) AS next2
    FROM Logs
) t
WHERE num = next1 AND num = next2;
```

Same rule as in [the DENSE_RANK note](dense_rank.md).

## `PARTITION BY` works here too

```sql
LEAD(num, 1) OVER (PARTITION BY user_id ORDER BY id)
```

"The next row **of the same user**." At each user's last row, `LEAD` gives
`NULL` in place of leaking into the next user's rows. This is the normal way to
measure the time gap between one user's events.

## Availability

MySQL **8.0+**, PostgreSQL, SQL Server 2012+, SQLite 3.25+, Oracle.
MySQL 5.7 does not have it — use a self-join there.
