-- LeetCode 180. Consecutive Numbers (Medium)
-- Find all numbers that appear at least three times in a row in Logs.
-- id is autoincrement, so row id+1 and id+2 are the next two rows.
-- Two correlated scalar subqueries look ahead one and two rows and compare num.

SELECT DISTINCT(num) AS  ConsecutiveNums from Logs L
where L.num = (select num from Logs P where P.id=(L.id+1))
AND L.num = (select num from Logs Q where Q.id=(L.id+2))
;

-- This solution is not O(N^2) because id is indexed .  
-- This actually runs in O(N) time .
-- ============================================================
-- SYNTAX LEARNED
-- ============================================================
--
-- 1. 'FROM Logs L' names the table L. L.num is Logs.num.
--    It saves typing, and it is REQUIRED when you use the same table twice.
--
-- 2. CORRELATED SUBQUERY: the inner query reads the outer row.
--       WHERE P.id = L.id + 1
--    L is the outer row, P is the inner one. At L.id = 1 the inner query
--    becomes 'WHERE P.id = 2'. It re-runs once per outer row.
--
-- 3. A SCALAR SUBQUERY must return at most one row.
--       L.num = (SELECT ...)
--    Two rows back gives 'Subquery returns more than 1 row' (error 1242).
--    Zero rows gives NULL.
--
-- 4. The two subqueries look ahead 1 and 2 rows. Both equal to L.num means
--    three equal values in a row.
--
-- 5. DISTINCT removes duplicate OUTPUT rows. On 3,3,3,3 both id=1 and id=2
--    qualify, so 3 appears twice, and DISTINCT collapses it.
--    DISTINCT does NOT force a subquery to return one row.
--
-- 6. AS renames the output column only. The data does not change.
--
-- 7. WHERE filters rows before grouping. HAVING filters after.
--    MySQL lets HAVING read a SELECT alias, and lets it run with no GROUP BY,
--    but WHERE is the right tool here.
--
--
-- ============================================================
-- MISTAKES MADE, AND THE FIX
-- ============================================================
--
-- MISTAKE 1: used a multi-row subquery on the right of '=', and expected
--            DISTINCT to reduce it to one value.
--   Error:  Subquery returns more than 1 row
--   Cause:  DISTINCT de-duplicates the OUTPUT of a query. It does not make a
--           subquery scalar. Those are two different jobs (see point 5).
--   Fix:    make the subquery match at most one row. P.id is the primary key,
--           so 'WHERE P.id = L.id + 1' can match one row at most.
--
-- MISTAKE 2: added a HAVING to drop NULLs.
--   Wrote:  HAVING ConsecutiveNums IS NOT NULL
--   Cause:  expected the last two rows to leak NULL into the output.
--   Why it was not needed: at the last row the subquery finds nothing and
--           gives NULL. Then 'L.num = NULL' is NULL, not TRUE, so WHERE
--           already dropped that row. The HAVING removed nothing.
--   Fix:    delete it. The query above is the cleaned version.
--
--
-- ============================================================
-- KNOWN WEAKNESS: this assumes id has NO GAPS
-- ============================================================
--
-- Autoincrement guarantees INCREASING ids, not CONTIGUOUS ids. A DELETE
-- leaves a hole. With a hole, 'P.id = L.id + 1' finds no row, gives NULL, and
-- a real run of three is missed.
-- LeetCode's data has no gaps, so this passes. Real data can break it.
--
--
-- ============================================================
-- ALTERNATE 1: LEAD() - does not use id arithmetic
-- ============================================================
--
-- SELECT DISTINCT num AS ConsecutiveNums
-- FROM (
--     SELECT num,
--            LEAD(num, 1) OVER (ORDER BY id) AS next1,
--            LEAD(num, 2) OVER (ORDER BY id) AS next2
--     FROM Logs
-- ) t
-- WHERE num = next1 AND num = next2;
--
-- LEAD(num, k) reads num from k rows later IN THE SORT ORDER, not from id + k.
-- So gaps in id stop mattering. It also sorts once, in place of two subqueries
-- per row. MySQL 8.0+.
--
-- ============================================================
-- ALTERNATE 2: self-join
-- ============================================================
-- Here the idea is to make the final table look like this:
-- l1.id	l1.num	l2.id	l2.num	l3.id	l3.num
-- 1         3       2       3       3       3
-- Each such record describes a (i,i+1,i+2) run of three equal numbers. The DISTINCT removes duplicates.\
-- For this we use self join 

-- SELECT distinct(L.num) AS ConsecutiveNUms from Logs L 
-- join Logs P ON P.id=L.id+1 AND L.num=P.num 
-- join Logs Q ON Q.id=L.id+2 AND Q.num=P.num ; 

This is also O(N) time . 
This is faster than the original solution of subqueries .  
However, MySQL executes the two queries in different ways.

Correlated subqueries

The correlated query checks the subqueries for each row of L.

For each row:

MySQL checks the P subquery.
MySQL checks the Q subquery.
Each subquery uses the value from the current L row.

Therefore, MySQL performs about 2N indexed lookups.

The subqueries also have some extra execution overhead because they depend on the current row of L.

Self-joins

The JOIN query is part of one query plan.

For each row of L:

MySQL uses the index on id to find P with id = L.id + 1.
MySQL checks that P.num = L.num.
MySQL uses the index on id to find Q with id = L.id + 2.
MySQL checks that Q.num = L.num.

MySQL can optimize the complete join plan and perform these operations as part of one execution plan.

Important point

Both queries can have similar Big-O complexity when id is indexed.

N rows
x
O(1) or O(log N) indexed lookups

The JOIN version can still be faster because it can have less execution overhead and gives the optimizer one complete join plan.

Therefore:

Correlated subqueries:
    indexed lookups + subquery overhead

Self-joins:
    indexed lookups + join execution

--
-- The planner sees one join tree in place of per-row subqueries.
-- It still assumes no gaps in id.
--
A genuinely good sollution below
-- ============================================================
-- ALTERNATE 3: gaps and islands - use this if the 3 ever changes
-- ============================================================
--
-- SELECT DISTINCT num AS ConsecutiveNums
-- FROM (
--     SELECT num,
--            ROW_NUMBER() OVER (ORDER BY id)
--          - ROW_NUMBER() OVER (PARTITION BY num ORDER BY id) AS grp
--     FROM Logs
-- ) t
-- GROUP BY num, grp
-- HAVING COUNT(*) >= 3;
--
-- Both counters step by 1 inside an unbroken run, so their DIFFERENCE stays
-- constant. That constant labels the run. Then count the rows of each run.
-- Change 3 to any K. The LEAD version would need K-1 more columns.
