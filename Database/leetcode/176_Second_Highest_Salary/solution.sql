-- select salary as SecondHighestSalary from Employee
--     order by salary DESC limit 1 offset 1;

-- This fails as it doesnt hadle cases where there are multiple
-- SecondHighest salaries and also if no such salary exists.



-- also offset is a part of where clause
-- hence if i wanted all records execept first then
-- I'm forced to use limit len-1
-- but actually no 

-- LIMIT 18446744073709551615 OFFSET 1;
-- I can have this instead signifying no bound on limit

-- Also to get all employees with second highest salary, we can use the following query:
-- SELECT id, salary
-- FROM Employee
-- WHERE salary < (SELECT MAX(salary) FROM Employee);

-- Btw group by collapses all records with same salary into one record, so if we want
-- to see what's inside each group

-- we need a AGGREGATE FUNTION 
-- For example, to see the IDs inside each salary group in MySQL:

-- SELECT salary, GROUP_CONCAT(id) AS ids
-- FROM Employee
-- GROUP BY salary;

-- SO the keyword is 'GROUP_CONCAT' which is MySQL.

-- but output of 'GROUP_CONCAT' is 
-- | ID  |
-- | --- |
-- | 1,2 |

-- so instead we can use nested query like
-- SELECT id
-- FROM Employee
-- WHERE salary = (
--     SELECT salary
--     FROM Employee
--     GROUP BY salary
--     ORDER BY salary DESC
--     LIMIT 1 OFFSET 1
-- );

SELECT
    (
        SELECT DISTINCT salary FROM Employee ORDER BY salary DESC LIMIT 1 OFFSET 1
    ) 
    AS SecondHighestSalary;

-- This involves sorting so 
-- O(N log N ) time complexity and O(N) space complexity . The space is storing 
-- the distinct salaries in the subquery.

-- As mentioned in readme a MAX() solution it will run in 
-- O(N)  but space is O(1) 


-- ALso 

-- Every derived table must have its own alias

-- so instead of 
-- SELECT (
--     SELECT salary 
--     FROM (
--         select salary,DENSE_RANK() OVER (ORDER BY salary DESC) AS RNK
--         FROM EMPLOYEE
--     ) where RNK = 2
--     LIMIT 1
-- )
-- AS SecondHighestSalary; 

-- we need to do
SELECT (
    SELECT salary 
    FROM (
        select salary,DENSE_RANK() OVER (ORDER BY salary DESC) AS RNK
        FROM EMPLOYEE
    ) AS ranked_employees
    WHERE RNK = 2
    LIMIT 1
)
AS SecondHighestSalary; 

-- The idea of using partition  is also pretty good 