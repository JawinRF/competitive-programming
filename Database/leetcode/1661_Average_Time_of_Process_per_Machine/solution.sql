-- LeetCode 1661. Average Time of Process per Machine (Easy)
-- Activity(machine_id, process_id, activity_type ENUM('start','end'), timestamp).
-- Each (machine_id, process_id) has exactly one 'start' row and one 'end' row.
-- For every machine, report the average of (end - start) over its processes,
-- rounded to 3 decimals, as processing_time.
-- Two grouping levels: per process first, then per machine.

select machine_id , ROUND(AVG(run_time),3) AS processing_time
    FROM (

        SELECT machine_id,
        MAX(
            CASE activity_type
                WHEN 'end' THEN timestamp
            END
        )
        -
        MAX(
            CASE activity_type
                WHEN 'start' THEN timestamp
            END
        )
        AS run_time
        FROM Activity
        GROUP BY machine_id,process_id

    ) t
    GROUP BY machine_id ;


-- ============================================================
-- THE MAIN LEARNING: EXECUTION ORDER DECIDES WHERE A SUBQUERY GOES
-- ============================================================
--
-- The logical order of one SELECT:
--
--    1  FROM / JOIN        a subquery here is a TABLE. It runs to completion
--                          FIRST, with its own GROUP BY.
--    2  WHERE              raw rows. No groups exist yet, so no aggregate.
--    3  GROUP BY           the rows collapse into groups.
--    4  aggregates         MAX, AVG, SUM run over a FINISHED group.
--    5  HAVING             groups, and their aggregates.
--    6  window functions
--    7  SELECT list        a subquery here runs NOW - AFTER the grouping.
--    8  DISTINCT
--    9  ORDER BY           the SELECT aliases exist by now.
--   10  LIMIT
--
-- Read steps 3 and 4 together. GROUP BY RUNS BEFORE THE AGGREGATE FUNCTIONS.
-- The aggregate never builds a group; it always receives a finished one.
-- So a subquery is never needed to "make" the group for an aggregate.
-- Write GROUP BY, and step 4 follows by itself.
--
-- The position of the subquery is the whole decision:
--
--   in FROM    step 1, FIRST   -> one finished level of grouping, ready to be
--                                grouped again outside.  <- used here
--   in SELECT  step 7, LATE    -> one value per output row. It cannot create
--                                groups, and it cannot group the outer query.
--
-- THE HINT TO KEEP:
--   Do not reach for a subquery to make a group. Write the GROUP BY.
--   Reach for a FROM subquery only to FINISH one level of grouping, so the
--   next level can group its result.
--
--
-- ============================================================
-- WHY TWO LEVELS
-- ============================================================
--
-- The task names two keys, so it needs two levels:
--
--   inner  GROUP BY machine_id, process_id  -> one row per process, run_time
--   outer  GROUP BY machine_id              -> one row per machine, the average
--
-- process_id is the FINER key, so it groups inside.
-- machine_id is the COARSER key, so it groups outside.
--
-- machine_id must ALSO sit in the inner GROUP BY. The outer query can use only
-- the columns the inner query hands out. Drop it there, and it is gone outside.
--
-- The CASE is the pivot of two rows into two columns:
--   CASE activity_type WHEN 'end' THEN timestamp END
-- returns NULL on every start row, and MAX ignores NULL. So each MAX picks the
-- single timestamp of its own type. MIN works the same here.
--
--
-- ============================================================
-- MISTAKES MADE, AND THE FIX
-- ============================================================
--
-- MISTAKE 1: put the work in a SELECT-list subquery, and expected it to run
--            once inside each machine_id group.
--
--   SELECT machine_id, (
--       select end_time-start_time from (
--           select (select timestamp from Activity where activity_type='start'
--                   GROUP BY process_id) as start_time,
--                  (select timestamp from Activity where activity_type='end'
--                   GROUP BY process_id) as end_time
--       ) t
--   )
--   from Activity;
--
--   Cause: step 7 above. A SELECT-list subquery runs after the grouping, and
--          this query has no GROUP BY at all. It returns one row per RAW row.
--   Three separate faults:
--     a. The inner queries name no outer column, so they are UNCORRELATED.
--        They return every start of the whole table, for all machines.
--        A subquery becomes correlated only when you write an outer column in
--        it, for example  WHERE x.process_id = a.process_id.
--     b. A multi-row subquery sits where one value fits.
--        Error 1242 - Subquery returns more than 1 row.
--     c. 'SELECT timestamp ... GROUP BY process_id' selects a bare column.
--        Error 1055 under ONLY_FULL_GROUP_BY.
--   Fix:   move the work into FROM, and group there. That is the query above.
--
-- MISTAKE 2: wrote two statements, split by ';', and expected the second one to
--            read start_time from the first.
--   Cause: ';' is a statement TERMINATOR. The server runs statement 1, sends
--          the result, and forgets it. An alias is not a variable, so it does
--          not survive the ';'. Two result sets are never one answer.
--   Fix:   one statement - a FROM subquery, a CTE, or conditional aggregation.
--
--
-- ============================================================
-- ALTERNATE 1: one level, by arithmetic
-- ============================================================
--
-- SELECT machine_id,
--        ROUND(SUM(CASE WHEN activity_type = 'end' THEN timestamp
--                       ELSE -timestamp END)
--              / COUNT(DISTINCT process_id), 3) AS processing_time
-- FROM Activity
-- GROUP BY machine_id;
--
-- Each 'end' adds and each 'start' subtracts, so the pairing needs no join and
-- no second level. One pass over the table.
-- It relies on the promise of exactly one start and one end per process.
--
-- ============================================================
-- ALTERNATE 2: self-join - the pairing is explicit
-- ============================================================
--
-- SELECT s.machine_id, ROUND(AVG(e.timestamp - s.timestamp), 3) AS processing_time
-- FROM Activity s
-- JOIN Activity e
--   ON  e.machine_id = s.machine_id
--   AND e.process_id = s.process_id
--   AND e.activity_type = 'end'
-- WHERE s.activity_type = 'start'
-- GROUP BY s.machine_id;
--
-- ============================================================
-- ALTERNATE 3: CTE - the same two levels, named
-- ============================================================
--
-- WITH runs AS (
--     SELECT machine_id, process_id,
--            MAX(CASE WHEN activity_type = 'end'   THEN timestamp END)
--          - MAX(CASE WHEN activity_type = 'start' THEN timestamp END) AS run_time
--     FROM Activity
--     GROUP BY machine_id, process_id
-- )
-- SELECT machine_id, ROUND(AVG(run_time), 3) AS processing_time
-- FROM runs
-- GROUP BY machine_id;
--
-- Identical work to the accepted answer. The name 'runs' says what the inner
-- level means, and no nesting hides it. Note the commas and the single ';'.
