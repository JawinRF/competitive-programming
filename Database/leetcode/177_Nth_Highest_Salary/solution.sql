-- LeetCode 177. Nth Highest Salary (Medium)
-- Return the Nth highest DISTINCT salary from Employee.
-- If fewer than N distinct salaries exist, return NULL.
-- DENSE_RANK() numbers the distinct salary levels, so RNK = N is the answer.

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


-- ============================================================
-- WHY THIS WORKS
-- ============================================================
--
-- DENSE_RANK() gives tied salaries the same number and leaves no gap.
-- So RNK = N means "the Nth highest distinct salary". No DISTINCT needed.
--
-- LIMIT 1 guards against ties: two employees on the Nth salary give two rows
-- with the same RNK, and the function must return one value.
--
-- The empty case is free. A scalar subquery that matches no row gives NULL,
-- so a too-large N returns NULL by itself.
--
-- N <= 0 is also safe. DENSE_RANK starts at 1, so RNK never equals 0 or less,
-- and the result is NULL.
--
-- NOTE: the outer 'select ( ... )' is redundant. RETURN already treats the
-- subquery as scalar and gives NULL for zero rows. This is enough:
--
--   RETURN (
--       select salary from (
--           select salary, DENSE_RANK() OVER (order by salary DESC) as RNK
--           from Employee
--       ) AS ranked_salary
--       where RNK = N
--       limit 1
--   );
--
--
-- ============================================================
-- ALTERNATE 1: LIMIT / OFFSET  (the 176 approach, adapted)
-- ============================================================
--
-- CREATE FUNCTION getNthHighestSalary(N INT) RETURNS INT
-- BEGIN
--   SET N = N - 1;
--   RETURN (
--       SELECT DISTINCT salary FROM Employee
--       ORDER BY salary DESC
--       LIMIT 1 OFFSET N
--   );
-- END
--
-- TRAP: MySQL does not accept an expression in LIMIT or OFFSET.
--   LIMIT 1 OFFSET N-1     -- syntax error
-- You must assign first with SET N = N - 1, then use the plain variable.
-- This is the main reason people fail this problem after they pass 176.
--
-- TRAP 2: if the caller passes N = 0, then OFFSET -1 raises an error.
-- The DENSE_RANK version returns NULL instead. LeetCode does not test N <= 0.
--
--
-- ============================================================
-- ALTERNATE 2: correlated subquery, no window function
-- ============================================================
--
-- Use this on MySQL 5.7 and older, where DENSE_RANK() does not exist.
--
-- CREATE FUNCTION getNthHighestSalary(N INT) RETURNS INT
-- BEGIN
--   RETURN (
--       SELECT DISTINCT e.salary
--       FROM Employee e
--       WHERE N - 1 = (
--           SELECT COUNT(DISTINCT e2.salary)
--           FROM Employee e2
--           WHERE e2.salary > e.salary
--       )
--   );
-- END
--
-- Idea: a salary is the Nth highest when exactly N-1 distinct salaries beat it.
-- It needs no LIMIT, so the expression N - 1 is legal here.
-- Cost is O(n^2), against O(n log n) for the other two. It is the slowest.
--
--
-- ============================================================
-- ALTERNATE 3: CTE, same idea as the accepted answer but flatter
-- ============================================================
--
-- CREATE FUNCTION getNthHighestSalary(N INT) RETURNS INT
-- BEGIN
--   RETURN (
--       WITH ranked AS (
--           SELECT salary, DENSE_RANK() OVER (ORDER BY salary DESC) AS rnk
--           FROM Employee
--       )
--       SELECT DISTINCT salary FROM ranked WHERE rnk = N
--   );
-- END
--
-- DISTINCT replaces LIMIT 1 here. All rows with rnk = N hold the same salary,
-- so DISTINCT collapses them to one value.
