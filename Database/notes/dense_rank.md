# `DENSE_RANK()` and the ranking window functions

## What `DENSE_RANK()` does

It is a **window function**. It gives each row a position number, in an order
you choose. Rows with equal values get the **same** number, and the next number
follows with **no gap**.

```sql
DENSE_RANK() OVER (ORDER BY salary DESC)
```

Read it as: "sort by salary, highest first, then number the rows."

## The three ranking functions

Same data, same order, three different answers. This table is the whole lesson:

| id | salary | `ROW_NUMBER()` | `RANK()` | `DENSE_RANK()` |
|---|---|---|---|---|
| 1 | 300 | 1 | 1 | 1 |
| 2 | 300 | 2 | 1 | 1 |
| 3 | 200 | 3 | **3** | **2** |
| 4 | 100 | 4 | 4 | 3 |

- **`ROW_NUMBER()`** — always 1, 2, 3, 4. It ignores ties and breaks them at
  random.
- **`RANK()`** — ties share a number, then it **skips**. Two rows hold rank 1,
  so rank 2 disappears. This is sport scoring: two gold medals, no silver.
- **`DENSE_RANK()`** — ties share a number, then it **continues**. No number is
  skipped.

The word *dense* means "no holes in the sequence".

## Why it matters for problem 176

`DENSE_RANK() = 2` is exactly "the second highest **distinct** salary". It does
the same job as `DISTINCT`, but it names the level instead of counting rows.

[Problem 176](../leetcode/176_Second_Highest_Salary/README.md), rewritten:

```sql
SELECT (
    SELECT salary
    FROM (
        SELECT salary, DENSE_RANK() OVER (ORDER BY salary DESC) AS rnk
        FROM Employee
    ) t
    WHERE rnk = 2
    LIMIT 1
) AS SecondHighestSalary;
```

Two points about this query:

1. **`LIMIT 1` is still necessary.** If two employees both earn 200, both rows
   get `rnk = 2`. You would get two rows of the same salary. `LIMIT 1` or
   `DISTINCT` fixes it.
2. The `LIMIT 1 OFFSET 1` version is shorter and faster here. `DENSE_RANK` wins
   when you need the **Nth** highest, or a rank per group.

## The trap that catches everyone

You **cannot** put a window function in `WHERE`:

```sql
-- ERROR
SELECT salary FROM Employee
WHERE DENSE_RANK() OVER (ORDER BY salary DESC) = 2;
```

`WHERE` runs **before** the window function does. The rank does not exist yet.
So you must compute the rank in a subquery or CTE first, then filter outside it:

```sql
WITH ranked AS (
    SELECT salary, DENSE_RANK() OVER (ORDER BY salary DESC) AS rnk
    FROM Employee
)
SELECT DISTINCT salary FROM ranked WHERE rnk = 2;
```

## `PARTITION BY` — restart the count per group

This is where window functions earn their keep. Add `PARTITION BY` to rank
**inside** each group:

```sql
SELECT name, dept, salary,
       DENSE_RANK() OVER (PARTITION BY dept ORDER BY salary DESC) AS rnk
FROM Employee;
```

The rank restarts at 1 for every department. To get the top earner of each
department, filter `rnk = 1`. Doing that with `LIMIT/OFFSET` is painful.

**Key difference from `GROUP BY`:** `GROUP BY` collapses rows. A window function
keeps every row and adds a column. You do not need `GROUP_CONCAT` to see group
members; a window function keeps them all.

## Availability

Needs MySQL **8.0+**, PostgreSQL, SQL Server 2012+, SQLite 3.25+, Oracle.
MySQL 5.7 does not have it.

## Problems that use this

- **176. Second Highest Salary** — `rnk = 2`
- **177. Nth Highest Salary** — `rnk = N`
- **178. Rank Scores** — `DENSE_RANK()` is the whole answer
