-- LeetCode 577. Employee Bonus (Easy)
-- Employee(empId, name, supervisor, salary), Bonus(empId, bonus).
-- Report the name and the bonus of every employee whose bonus is below 1000.
-- An employee with no row in Bonus has no bonus, so that employee counts too.
-- LEFT JOIN keeps those employees and makes bonus NULL.
-- A missing row is not a zero, so the test needs 'IS NULL' beside '< 1000'.

SELECT s.name,t.bonus FROM
Employee s
LEFT JOIN
Bonus t
ON s.empID = t.empId
having t.bonus is NULL OR t.bonus<1000 ;
