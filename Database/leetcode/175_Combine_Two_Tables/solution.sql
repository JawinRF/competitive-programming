-- LeetCode 175. Combine Two Tables (Easy)
-- Report firstName, lastName, city, state for every person.
-- If a person has no row in Address, report NULL for city and state.
-- LEFT JOIN keeps all Person rows and fills missing Address columns with NULL.

select p.firstName,p.lastName,q.city,q.state
    from Person p LEFT JOIN Address q
    on p.personId=q.personId ;


-- ============================================================
-- NOTE: how to show your own value in place of NULL
-- ============================================================
--
-- NULL is not a value. It is a marker for "no data".
-- To show something else, wrap the column in a function.
--
-- COALESCE is standard SQL. It works on MySQL, PostgreSQL,
-- SQL Server, SQLite and Oracle. Prefer it.
-- It returns the first argument that is not NULL.
--
--   select p.firstName,
--          p.lastName,
--          COALESCE(q.city,  'None') as city,
--          COALESCE(q.state, 'None') as state
--   from Person p LEFT JOIN Address q
--     on p.personId = q.personId;
--
-- COALESCE accepts more than two arguments:
--   COALESCE(q.city, q.state, 'Unknown')
--
-- Other forms (same idea, less portable):
--   IFNULL(x, 'None')   MySQL, SQLite. Two arguments only.
--   ISNULL(x, 'None')   SQL Server. In MySQL ISNULL() returns 0 or 1 instead.
--   NVL(x, 'None')      Oracle. Two arguments only.
--   CASE WHEN x IS NULL THEN 'None' ELSE x END   All databases. Long, but
--                                                it accepts any condition.
--
-- Trap 1: watch the data type.
--   city is varchar, so 'None' fits.
--   COALESCE(salary, 0)       good: number for number.
--   COALESCE(salary, 'None')  risky: it mixes number and text.
--   Cast first if you need text: COALESCE(CAST(salary AS CHAR), 'None')
--
-- Trap 2: never test NULL with '='.
--   where q.city IS NULL     correct
--   where q.city = NULL      wrong, it returns no rows
--
-- For THIS problem, keep NULL. The judge compares the output to a table that
-- holds Null. COALESCE(q.city,'None') writes the text 'None', which is a
-- different value, so the judge marks it wrong.
-- Use COALESCE when the task asks for it, or in real reports where a reader
-- must see a word in place of an empty cell.
