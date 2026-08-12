# 1661. Average Time of Process per Machine

**Source:** [LeetCode](https://leetcode.com/problems/average-time-of-process-per-machine/)
**Difficulty:** Easy
**Topic:** Database
**Status:** Accepted

---

## Problem

Table: `Activity`

```
+----------------+---------+
| Column Name    | Type    |
+----------------+---------+
| machine_id     | int     |
| process_id     | int     |
| activity_type  | enum    |
| timestamp      | float   |
+----------------+---------+
```

`(machine_id, process_id, activity_type)` is the primary key.
`activity_type` is an ENUM of type `('start', 'end')`.
`timestamp` is a float, the time in seconds.
The `start` timestamp is always less than or equal to the `end` timestamp of the
same `(machine_id, process_id)` pair.
Every `(machine_id, process_id)` pair is guaranteed to have both a `start` and an
`end` row.

**Task:** Find the average time each machine takes to complete a process.
The time of one process is `end` minus `start`. The average is the total time of
every process on the machine, divided by the number of processes. Name the
column `processing_time` and round it to 3 decimal places. Return the rows in
any order.

---

## Example 1

**Input:**

`Activity` table:

```
+------------+------------+---------------+-----------+
| machine_id | process_id | activity_type | timestamp |
+------------+------------+---------------+-----------+
| 0          | 0          | start         | 0.712     |
| 0          | 0          | end           | 1.520     |
| 0          | 1          | start         | 3.140     |
| 0          | 1          | end           | 4.120     |
| 1          | 0          | start         | 0.550     |
| 1          | 0          | end           | 1.550     |
| 1          | 1          | start         | 0.430     |
| 1          | 1          | end           | 1.420     |
| 2          | 0          | start         | 4.100     |
| 2          | 0          | end           | 4.512     |
| 2          | 1          | start         | 2.500     |
| 2          | 1          | end           | 5.000     |
+------------+------------+---------------+-----------+
```

**Output:**

```
+------------+-----------------+
| machine_id | processing_time |
+------------+-----------------+
| 0          | 0.894           |
| 1          | 0.995           |
| 2          | 1.456           |
+------------+-----------------+
```

**Explanation:** machine 0 runs two processes, of 0.808 s and 0.980 s.
`(0.808 + 0.980) / 2 = 0.894`.

---

## Accepted solution

```sql
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
```

See [solution.sql](solution.sql).

The problem needs **two** levels of grouping, because it names two keys:

| Level | Key | Produces |
|---|---|---|
| inner | `machine_id, process_id` | one row per process, holding its `run_time` |
| outer | `machine_id` | one row per machine, holding the average |

`process_id` is the **finer** key, so it groups inside. `machine_id` is the
**coarser** key, so it groups outside. `machine_id` must also sit in the inner
`GROUP BY`, because the outer query can use only the columns the inner query
hands out.

`CASE activity_type WHEN 'end' THEN timestamp END` returns `NULL` on every start
row. `MAX` ignores `NULL`. So each of the two `MAX` calls picks the one
timestamp of its own type. This is the standard pivot of two rows into two
columns.

---

## The main learning — execution order decides where a subquery goes

The first attempt tried to build the groups with a subquery in the `SELECT`
list. That cannot work, and the reason is the order the clauses run in.

**The logical order of one `SELECT`:**

| # | Step | What it can see |
|---|---|---|
| 1 | `FROM`, `JOIN` | a subquery here is a **table**. It runs to completion first, with its own `GROUP BY` |
| 2 | `WHERE` | raw rows. No groups exist yet, so no aggregate is allowed |
| 3 | **`GROUP BY`** | the rows collapse into groups |
| 4 | **aggregates** | `MAX`, `AVG`, `SUM` run over a group that is already complete |
| 5 | `HAVING` | groups, and their aggregates |
| 6 | window functions | the grouped rows |
| 7 | `SELECT` list | a subquery here runs **now** — after the grouping |
| 8 | `DISTINCT` | |
| 9 | `ORDER BY` | the `SELECT` aliases exist by now |
| 10 | `LIMIT` | |

Read steps 3 and 4 together. **`GROUP BY` runs before the aggregate functions.**
The aggregate never builds a group; it always receives a finished one. So you
never need a subquery to "make" the group for an aggregate. You write
`GROUP BY`, and step 4 follows by itself.

**The position of the subquery is the whole decision:**

| Subquery position | Step | Use it for |
|---|---|---|
| in `FROM` | **1 — first** | one finished level of grouping, to be grouped again outside |
| in `SELECT` | **7 — after the grouping** | one value per output row. It cannot create groups |

The accepted answer puts the subquery in `FROM` for exactly this reason. The
inner `GROUP BY machine_id, process_id` is complete before the outer query
starts, so `AVG(run_time)` at step 4 of the outer query reads finished numbers.

**The hint to keep:**

> Do not reach for a subquery to make a group. Write the `GROUP BY`. Reach for a
> `FROM` subquery only to **finish one level** of grouping, so the next level
> can group its result.

---

## Mistakes made, and the fix

### Mistake 1 — a subquery in `SELECT`, expected to run "inside each group"

**Wrote:**

```sql
SELECT machine_id, (
    select end_time-start_time from (
        select (select timestamp from Activity where activity_type='start'
                GROUP BY process_id) as start_time,
               (select timestamp from Activity where activity_type='end'
                GROUP BY process_id) as end_time
    ) t
)
from Activity;
```

**Cause:** it expects the subquery to run once per `machine_id` group. Step 7
above shows it runs after the grouping, and this query has no `GROUP BY` at all.

**Three separate faults:**

| Fault | Result |
|---|---|
| The inner queries name no outer column, so they are **uncorrelated** | they return every start of the whole table, for all machines |
| A multi-row subquery sits where one value fits | error **1242 — Subquery returns more than 1 row** |
| `SELECT timestamp ... GROUP BY process_id` selects a bare column | error **1055** under `ONLY_FULL_GROUP_BY` |

**Fix:** move the work into `FROM`, and group there. That is the accepted
answer.

### Mistake 2 — expected `;` to carry a value forward

**Wrote:** two statements, one for the starts and one for the ends, split by `;`.

**Cause:** `;` is a statement terminator. The server runs statement 1, sends the
result, and forgets it. `start_time` is an alias, not a variable, so it does not
survive. Two result sets are never one answer.

**Fix:** one statement. Use a `FROM` subquery, a CTE, or conditional
aggregation.

---

## Alternate 1 — one level, by arithmetic

Every process holds exactly one start and one end. So the average is the signed
sum, divided by the number of processes:

```sql
SELECT machine_id,
       ROUND(SUM(CASE WHEN activity_type = 'end' THEN timestamp ELSE -timestamp END)
             / COUNT(DISTINCT process_id), 3) AS processing_time
FROM Activity
GROUP BY machine_id;
```

Each `end` adds and each `start` subtracts, so the pairing needs no join and no
second level. One pass over the table.

## Alternate 2 — self-join

```sql
SELECT s.machine_id, ROUND(AVG(e.timestamp - s.timestamp), 3) AS processing_time
FROM Activity s
JOIN Activity e
  ON  e.machine_id = s.machine_id
  AND e.process_id = s.process_id
  AND e.activity_type = 'end'
WHERE s.activity_type = 'start'
GROUP BY s.machine_id;
```

The pairing is explicit here, so it reads well. It also stays correct if a
process ever holds more than two rows.

## Alternate 3 — CTE

The same two levels, named:

```sql
WITH runs AS (
    SELECT machine_id, process_id,
           MAX(CASE WHEN activity_type = 'end'   THEN timestamp END)
         - MAX(CASE WHEN activity_type = 'start' THEN timestamp END) AS run_time
    FROM Activity
    GROUP BY machine_id, process_id
)
SELECT machine_id, ROUND(AVG(run_time), 3) AS processing_time
FROM runs
GROUP BY machine_id;
```

Identical work to the accepted answer. The name `runs` says what the inner level
means, and no nesting hides it.

---

## Compare

| Approach | Passes over `Activity` | Needs the exact one start / one end promise |
|---|---|---|
| Two-level `FROM` subquery (accepted) | 1 group, then 1 group | no |
| One level by arithmetic | **1** | **yes** |
| Self-join | 1 join, then 1 group | no |
| CTE | same as accepted | no |

See also: [what GROUP BY collapses](../../notes/group_by_collapse.md) and
[the ENUM note](../../notes/enum.md).
