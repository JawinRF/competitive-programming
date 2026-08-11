# 176. Second Highest Salary

**Source:** [LeetCode](https://leetcode.com/problems/second-highest-salary/)
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

**Task:** Find the second highest **distinct** salary from the `Employee` table.
If there is no second highest salary, return `null` (return `None` in Pandas).

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
```

**Output:**

```
+---------------------+
| SecondHighestSalary |
+---------------------+
| 200                 |
+---------------------+
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
```

**Output:**

```
+---------------------+
| SecondHighestSalary |
+---------------------+
| null                |
+---------------------+
```

---

## Solution

```sql
SELECT
    (
        SELECT DISTINCT salary FROM Employee ORDER BY salary DESC LIMIT 1 OFFSET 1
    )
    AS SecondHighestSalary;
```

See [solution.sql](solution.sql).

---

## Idea

Three parts do three jobs.

**1. `ORDER BY salary DESC LIMIT 1 OFFSET 1` picks the second row.**
`OFFSET 1` skips the top salary. `LIMIT 1` takes the next one.

**2. `DISTINCT` collapses ties.**
Without it, two employees on the top salary make the second row the *same*
salary as the first. The task asks for the second highest **distinct** salary.

**3. The outer `SELECT ( ... )` handles the empty case.**
This is the part that is easy to miss. Run the inner query alone on Example 2
and it returns **zero rows**, but the expected output is **one row holding
`null`**. A scalar subquery in the select list gives `NULL` when it matches no
row, so the wrapper turns zero rows into one `NULL` row.

## Alternative

`MAX` over an empty set is `NULL`, so this needs no wrapper:

```sql
SELECT MAX(salary) AS SecondHighestSalary
FROM Employee
WHERE salary < (SELECT MAX(salary) FROM Employee);
```

## Notes

- `LIMIT` and `OFFSET` are not standard SQL. They work on MySQL, PostgreSQL and
  SQLite. SQL Server uses `OFFSET 1 ROWS FETCH NEXT 1 ROWS ONLY`.
- In MySQL, `OFFSET` needs a `LIMIT` beside it. To skip rows with no upper
  bound, use the largest `BIGINT`:
  `LIMIT 18446744073709551615 OFFSET 1`.
- `GROUP BY salary` collapses each salary into one row. To see the members of a
  group, you need an aggregate function, for example
  `GROUP_CONCAT(id)` in MySQL. That returns `1,2` in one cell, not two rows.
- To list every employee **on** the second highest salary, feed the subquery to
  a `WHERE`:
  ```sql
  SELECT id FROM Employee
  WHERE salary = (
      SELECT DISTINCT salary FROM Employee ORDER BY salary DESC LIMIT 1 OFFSET 1
  );
  ```
