SELECT
    E.employee_id,
    E.name,
    COUNT(E.employee_id) AS reports_count,
    ROUND(AVG(T.age)) AS average_age
FROM Employees E
INNER JOIN
(
    SELECT age, reports_to
    FROM Employees
) AS T
    ON E.employee_id = T.reports_to
GROUP BY E.employee_id
ORDER BY E.employee_id;
