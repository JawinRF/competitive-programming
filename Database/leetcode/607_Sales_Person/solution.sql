SELECT name
FROM SalesPerson
LEFT OUTER JOIN
(
    SELECT sales_id
    FROM Orders O
    INNER JOIN
    (
        SELECT com_id
        FROM Company
        WHERE name = 'RED'
    ) AS C
    ON O.com_id = C.com_id
) AS T
ON SalesPerson.sales_id = T.sales_id
WHERE T.sales_id IS NULL;
