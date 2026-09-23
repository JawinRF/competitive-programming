-- SELECT name
-- FROM EMPLOYEE e
-- WHERE e.salary > MANAGER'S SALARY

SELECT e.name AS Employee
FROM EMPLOYEE e
INNER JOIN EMPLOYEE M
    ON e.managerId = M.id
    AND e.salary > M.salary;
