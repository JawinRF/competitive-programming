-- LeetCode 1581. Customer Who Visited but Did Not Make Any Transactions (Easy)
-- Visits(visit_id, customer_id), Transactions(transaction_id, visit_id, amount).
-- For each customer, count the visits that made no transaction.
-- Report only the customers with at least one such visit.
-- LEFT JOIN plus 'WHERE t.transaction_id IS NULL' is the anti-join: it keeps the
-- visits that found no partner row in Transactions.
-- After that filter, one row is one empty visit, so COUNT(*) counts empty visits.

SELECT customer_id,count(*) AS count_no_trans
FROM Visits v
LEFT JOIN
Transactions t ON
v.visit_id = t.visit_id
where t.transaction_id is NULL
GROUP BY customer_id
ORDER BY customer_id ;
