# 180. Consecutive Numbers

**Source:** [LeetCode](https://leetcode.com/problems/consecutive-numbers/)
**Difficulty:** Medium
**Topic:** Database
**Status:** Accepted

---

## Problem

Table: `Logs`

```
+-------------+---------+
| Column Name | Type    |
+-------------+---------+
| id          | int     |
| num         | varchar |
+-------------+---------+
```

In SQL, `id` is the primary key for this table.
`id` is an autoincrement column starting from 1.

**Task:** Find all numbers that appear at least three times consecutively.
Return the result table in any order.

---

## Example 1

**Input:**

`Logs` table:

```
+----+-----+
| id | num |
+----+-----+
| 1  | 1   |
| 2  | 1   |
| 3  | 1   |
| 4  | 2   |
| 5  | 1   |
| 6  | 2   |
| 7  | 2   |
+----+-----+
```

**Output:**

```
+-----------------+
| ConsecutiveNums |
+-----------------+
| 1               |
+-----------------+
```

**Explanation:** 1 is the only number that appears consecutively at least three
times.

---

## Accepted solution

```sql
SELECT DISTINCT(num) AS  ConsecutiveNums from Logs L
where L.num = (select num from Logs P where P.id=(L.id+1))
AND L.num = (select num from Logs Q where Q.id=(L.id+2))
;
```

See [solution.sql](solution.sql).

**The key line of the statement is in the schema, not the task:** `id` is
autoincrement. So the next two rows are `id + 1` and `id + 2`. Two correlated
subqueries look ahead and compare `num`.

---

## Syntax learned

| # | Point |
|---|---|
| 1 | `FROM Logs L` names the table `L`. It saves typing, and it is **required** when you use the same table twice. |
| 2 | **Correlated subquery** — the inner query reads the outer row. `WHERE P.id = L.id + 1` becomes `WHERE P.id = 2` when `L.id = 1`. It re-runs once per outer row. |
| 3 | A **scalar subquery** must return at most one row. Two rows gives *"Subquery returns more than 1 row"*. Zero rows gives `NULL`. |
| 4 | The two subqueries look ahead 1 and 2 rows. Both equal to `L.num` means three equal values in a row. |
| 5 | `DISTINCT` removes duplicate **output rows**. On `3,3,3,3` both `id=1` and `id=2` qualify, so `3` appears twice and `DISTINCT` collapses it. It does **not** make a subquery scalar. |
| 6 | `AS` renames the output column only. The data does not change. |
| 7 | `WHERE` filters rows before grouping; `HAVING` filters after. MySQL lets `HAVING` read a `SELECT` alias and run with no `GROUP BY`, but `WHERE` is the right tool here. |

---

## Mistakes made, and the fix

### Mistake 1 — expected `DISTINCT` to make a subquery scalar

**Error:** `Subquery returns more than 1 row`

**Cause:** a multi-row subquery sat on the right of `=`. `DISTINCT`
de-duplicates the **output of a query**; it does not make a subquery return one
value. Two different jobs — this is point 5 above.

**Fix:** make the subquery match at most one row by keying on the primary key.
`P.id` is the primary key, so `WHERE P.id = L.id + 1` matches one row at most.

### Mistake 2 — added a `HAVING` that removed nothing

**Wrote:** `HAVING ConsecutiveNums IS NOT NULL`

**Cause:** expected the last two rows to leak `NULL` into the output.

**Why it was not needed:** at the last row the subquery finds nothing and gives
`NULL`. Then `L.num = NULL` evaluates to `NULL`, not `TRUE`, so `WHERE` already
dropped that row. The `HAVING` filtered nothing.

**Fix:** delete it. The accepted query above is the cleaned version.

---

## Known weakness — this assumes `id` has no gaps

Autoincrement guarantees **increasing** ids, not **contiguous** ids. A `DELETE`
leaves a hole. With a hole, `P.id = L.id + 1` finds no row, gives `NULL`, and a
real run of three is missed.

LeetCode's data has no gaps, so this passes. Real data can break it.

---

## Alternate 1 — `LEAD()`, no id arithmetic

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

`LEAD(num, k)` reads `num` from *k rows later in the sort order*, not from
`id + k`. Gaps in `id` stop mattering. It also sorts once, in place of two
subqueries per row. Needs MySQL 8.0+.

## Alternate 2 — self-join

```sql
SELECT DISTINCT l1.num AS ConsecutiveNums
FROM Logs l1
JOIN Logs l2 ON l2.id = l1.id + 1 AND l2.num = l1.num
JOIN Logs l3 ON l3.id = l1.id + 2 AND l3.num = l1.num;
```

Same logic as the accepted answer, but the planner sees one join tree in place
of per-row subqueries. It still assumes no gaps.

## Alternate 3 — gaps and islands

Use this if the `3` ever changes.

```sql
SELECT DISTINCT num AS ConsecutiveNums
FROM (
    SELECT num,
           ROW_NUMBER() OVER (ORDER BY id)
         - ROW_NUMBER() OVER (PARTITION BY num ORDER BY id) AS grp
    FROM Logs
) t
GROUP BY num, grp
HAVING COUNT(*) >= 3;
```

Both counters step by 1 inside an unbroken run, so their **difference stays
constant**. That constant labels the run. Then count the rows of each run.

On the example: the three `1`s all get `grp = 0`, count 3. The `2`s split into
`grp = 3` (count 1) and `grp = 4` (count 2). Output: `1`.

---

## Compare

| Approach | Survives id gaps | Scales to K |
|---|---|---|
| Correlated subqueries (accepted) | no | no |
| Self-join | no | no |
| `LEAD` | **yes** | no |
| Gaps and islands | **yes** | **yes** |

See also: [DENSE_RANK note](../../notes/dense_rank.md).
