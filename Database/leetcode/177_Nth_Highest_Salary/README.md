# 177. Nth Highest Salary

**Source:** [LeetCode](https://leetcode.com/problems/nth-highest-salary/)
**Difficulty:** Medium
**Topic:** Database
**Status:** Accepted

---

## Problem

Table: `Employee`

```
+-------------+------+
| Column Name | Type |
+-------------+------+
| id          | int  |
| salary      | int  |
+-------------+------+
```

`id` is the primary key (column with unique values) for this table.
Each row contains the salary of one employee.

**Task:** Find the `nth` highest **distinct** salary from the `Employee` table.
If there are less than `n` distinct salaries, return `null`.

---

## Example 1

**Input:**

`Employee` table:

```
+----+--------+
| id | salary |
+----+--------+
| 1  | 100    |
| 2  | 200    |
| 3  | 300    |
+----+--------+
n = 2
```

**Output:**

```
+------------------------+
| getNthHighestSalary(2) |
+------------------------+
| 200                    |
+------------------------+
```

## Example 2

**Input:**

`Employee` table:

```
+----+--------+
| id | salary |
+----+--------+
| 1  | 100    |
+----+--------+
n = 2
```

**Output:**

```
+------------------------+
| getNthHighestSalary(2) |
+------------------------+
| null                   |
+------------------------+
```

---

## Accepted solution — `DENSE_RANK()`

```sql
CREATE FUNCTION getNthHighestSalary(N INT) RETURNS INT
BEGIN
  RETURN (
      # Write your MySQL query statement below.
      select (
        select salary from
            (
                select salary,DENSE_RANK() OVER (order by salary DESC) as RNK
                from Employee
            ) AS ranked_salary
            where RNK = N
            limit 1
      )
  );
END
```

See [solution.sql](solution.sql).

### Why it works

`DENSE_RANK()` gives tied salaries the same number and leaves no gap. So
`RNK = N` means "the Nth highest **distinct** salary". You need no `DISTINCT`.
See the [DENSE_RANK note](../../notes/dense_rank.md).

Three cases come out for free:

| Case | Result | Reason |
|---|---|---|
| Ties on the Nth salary | one value | `LIMIT 1` picks one of the equal rows |
| `N` larger than the count | `NULL` | a scalar subquery with no row gives `NULL` |
| `N <= 0` | `NULL` | `DENSE_RANK` starts at 1, so `RNK` never matches |

**One simplification:** the outer `select ( ... )` is redundant. `RETURN`
already treats the subquery as scalar, and gives `NULL` for zero rows.

---

## Alternate 1 — `LIMIT` / `OFFSET`

This is the [problem 176](../176_Second_Highest_Salary/README.md) approach,
adapted to a variable `N`.

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

**The trap:** MySQL does not accept an expression in `LIMIT` or `OFFSET`.

```sql
LIMIT 1 OFFSET N-1     -- syntax error
```

You must assign first with `SET N = N - 1`, then use the plain variable. This
is the main reason people fail 177 straight after they pass 176.

The restriction covers the `LIMIT` clause **only**, not SQL arithmetic in
general. Full explanation:
[notes/limit_offset_arithmetic.md](../../notes/limit_offset_arithmetic.md).

**Second trap:** `N = 0` makes `OFFSET -1`, which raises an error. The
`DENSE_RANK` version returns `NULL` instead. LeetCode does not test `N <= 0`.

---

## Alternate 2 — correlated subquery, no window function

Use this on MySQL 5.7 and older, where `DENSE_RANK()` does not exist.

```sql
CREATE FUNCTION getNthHighestSalary(N INT) RETURNS INT
BEGIN
  RETURN (
      SELECT DISTINCT e.salary
      FROM Employee e
      WHERE N - 1 = (
          SELECT COUNT(DISTINCT e2.salary)
          FROM Employee e2
          WHERE e2.salary > e.salary
      )
  );
END
```

**Idea:** a salary is the Nth highest when exactly `N-1` distinct salaries beat
it. It uses no `LIMIT`, so the expression `N - 1` is legal here.

---

## Alternate 3 — CTE

Same idea as the accepted answer, but flatter to read.

```sql
CREATE FUNCTION getNthHighestSalary(N INT) RETURNS INT
BEGIN
  RETURN (
      WITH ranked AS (
          SELECT salary, DENSE_RANK() OVER (ORDER BY salary DESC) AS rnk
          FROM Employee
      )
      SELECT DISTINCT salary FROM ranked WHERE rnk = N
  );
END
```

`DISTINCT` replaces `LIMIT 1`. All rows with `rnk = N` hold the same salary, so
`DISTINCT` collapses them to one value.

---

## Compare

| Approach | Cost | Needs MySQL 8.0+ | Note |
|---|---|---|---|
| `DENSE_RANK` (accepted) | O(n log n) | yes | Handles every edge case by itself |
| `LIMIT` / `OFFSET` | O(n log n) | no | Needs `SET N = N - 1`; breaks on `N <= 0` |
| Correlated subquery | O(n²) | no | Slowest; works on old MySQL |
| CTE | O(n log n) | yes | Clearest to read |
