# 175. Combine Two Tables

**Source:** [LeetCode](https://leetcode.com/problems/combine-two-tables/)
**Difficulty:** Easy
**Topic:** Database

---

## Problem

Table: `Person`

```
+-------------+---------+
| Column Name | Type    |
+-------------+---------+
| personId    | int     |
| lastName    | varchar |
| firstName   | varchar |
+-------------+---------+
```

`personId` is the primary key (column with unique values) for this table.
This table contains the ID of some persons and their first and last names.

Table: `Address`

```
+-------------+---------+
| Column Name | Type    |
+-------------+---------+
| addressId   | int     |
| personId    | int     |
| city        | varchar |
| state       | varchar |
+-------------+---------+
```

`addressId` is the primary key (column with unique values) for this table.
Each row contains the city and state of one person with ID = `personId`.

**Task:** Report the first name, last name, city, and state of each person in the
`Person` table. If the address of a `personId` is not present in the `Address`
table, report `null` instead.

Return the result table in **any order**.

---

## Example 1

**Input:**

`Person` table:

```
+----------+----------+-----------+
| personId | lastName | firstName |
+----------+----------+-----------+
| 1        | Wang     | Allen     |
| 2        | Alice    | Bob       |
+----------+----------+-----------+
```

`Address` table:

```
+-----------+----------+---------------+------------+
| addressId | personId | city          | state      |
+-----------+----------+---------------+------------+
| 1         | 2        | New York City | New York   |
| 2         | 3        | Leetcode      | California |
+-----------+----------+---------------+------------+
```

**Output:**

```
+-----------+----------+---------------+----------+
| firstName | lastName | city          | state    |
+-----------+----------+---------------+----------+
| Allen     | Wang     | Null          | Null     |
| Bob       | Alice    | New York City | New York |
+-----------+----------+---------------+----------+
```

**Explanation:**
There is no address in the `Address` table for `personId = 1`, so we return null
for their city and state. `addressId = 1` holds the address of `personId = 2`.

---

## Solution

```sql
select p.firstName,p.lastName,q.city,q.state
    from Person p LEFT JOIN Address q
    on p.personId=q.personId ;
```

See [solution.sql](solution.sql).

---

## Idea

The task says *every* person must appear in the output, with or without an
address. This is exactly what a `LEFT JOIN` does.

- `LEFT JOIN` keeps all rows of the left table (`Person`).
- For each `Person` row, it looks for `Address` rows with the same `personId`.
- If no `Address` row matches, SQL fills `q.city` and `q.state` with `NULL`.

An `INNER JOIN` is wrong here: it drops `personId = 1`, because that person has
no address.

The row `addressId = 2` (`personId = 3`) does not appear in the output. A
`LEFT JOIN` ignores right-table rows that match no left-table row.

## Notes

- `personId` is the primary key of `Person`, but it is **not** unique in
  `Address`. If one person had two addresses, the join would give two output
  rows for that person. The LeetCode data does not test this case.
- Column order in the `select` list sets the output column order:
  `firstName, lastName, city, state`.
