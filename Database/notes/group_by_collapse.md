# What `GROUP BY` collapses, and what happens to the other columns

## The collapse

`GROUP BY` does two things, in order:

1. **Split** the rows into groups. Rows with the same key value go together.
2. **Emit one row per group.**

Step 2 is the collapse. Many input rows become one output row.

```
id | machine | worker | salary
---|---------|--------|-------
1  | A       | John   | 50000
2  | A       | Bob    | 60000
3  | A       | Sam    | 55000
4  | B       | Alice  | 70000
```

```sql
SELECT machine FROM employees GROUP BY machine;
```

```
Machine A:  row 1 John 50000
            row 2 Bob  60000     →   one output row: A
            row 3 Sam  55000

Machine B:  row 4 Alice 70000    →   one output row: B
```

```
machine
-------
A
B
```

## Where `worker` and `salary` went

Nowhere. They are still inside the group. The problem is not that they are lost.
The problem is that there are **three** of each, and the output row holds
**one** cell:

| Column | Values in group A | One value to print? |
|---|---|---|
| `machine` | A, A, A | yes — the key is the same by definition |
| `worker` | John, Bob, Sam | **no** |
| `salary` | 50000, 60000, 55000 | **no** |

The key column is safe because grouping made it identical. Every other column
holds a set, and no set has one correct member.

> The collapse does not delete the other columns. It removes the single value
> they used to have.

That is why SQL forces you to say **which** value you mean. An aggregate is that
answer: it turns the set into one number.

```sql
SELECT machine,
       COUNT(*)    AS workers,
       AVG(salary) AS average_salary
FROM employees
GROUP BY machine;
```

## The rule applies only to columns you mention

This is the part that is easy to get backwards. The restriction is **not** "the
table may hold no other columns". It is:

> Every column **named in the `SELECT` list** must be either the group key, or
> inside an aggregate.

`employees` still has `id`, `worker` and `salary` while you group by `machine`.
None of them cause trouble, because the query never asks to print them. Ask for
one, and the query breaks:

```sql
SELECT machine, worker FROM employees GROUP BY machine;   -- error
SELECT machine          FROM employees GROUP BY machine;  -- fine
SELECT machine, MAX(worker) FROM employees GROUP BY machine;  -- fine
```

The same check covers `HAVING` and `ORDER BY`. `WHERE` is different — it runs
**before** the grouping, so it still sees the raw rows and every column in them.

## MySQL — error 1055, and the silent version

| Setting | `SELECT machine, worker ... GROUP BY machine` |
|---|---|
| `ONLY_FULL_GROUP_BY` on (MySQL 8 default) | **Error 1055.** `worker` is not in `GROUP BY` and is not aggregated |
| `ONLY_FULL_GROUP_BY` off (old MySQL) | Runs. MySQL prints **an arbitrary** worker of the group |

The second row is the dangerous one. Old MySQL accepted this, and old tutorials
still teach it. The answer is not wrong at random — it is wrong quietly, and it
changes when the storage order changes.

If you really want any member, say so, and the reader sees your intent:

```sql
SELECT machine, ANY_VALUE(worker) FROM employees GROUP BY machine;
```

## The exception — one value by definition

A column is allowed with no aggregate when the group key **determines** it.
Group by a primary key, and every other column of that table has exactly one
value per group:

```sql
SELECT e.id, e.worker, COUNT(*) AS shifts   -- legal: id is the primary key
FROM employees e
JOIN shifts s ON s.worker_id = e.id
GROUP BY e.id;
```

MySQL 8 recognises this and allows it. The rule never changed — such a column
already holds one value, so nothing must be chosen.

## The contrast — window functions do not collapse

| | `GROUP BY` | `OVER ( ... )` |
|---|---|---|
| Rows out | one per group | one per input row, all kept |
| Other columns | must be aggregated | stay as they are |

`AVG(salary) OVER (PARTITION BY machine)` gives every worker their machine's
average, beside their own name and salary. Nothing collapses, so nothing must be
chosen. See [the DENSE_RANK note](dense_rank.md).

## Watch out

- The error names the column, not the mistake. Read "1055 ... `worker` is not
  functionally dependent" as "you asked for one worker, and I hold three".
- `SELECT *` with `GROUP BY` is the same mistake, written shorter.
- `COUNT(*)` counts rows in the group. `COUNT(salary)` counts non-`NULL`
  salaries. They differ as soon as one value is `NULL`.
- Adding a column to `GROUP BY` to silence the error changes the answer. It
  makes finer groups, and therefore more rows.
