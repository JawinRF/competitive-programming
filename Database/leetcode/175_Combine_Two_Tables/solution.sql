-- LeetCode 175. Combine Two Tables (Easy)
-- Report firstName, lastName, city, state for every person.
-- If a person has no row in Address, report NULL for city and state.
-- LEFT JOIN keeps all Person rows and fills missing Address columns with NULL.

select p.firstName,p.lastName,q.city,q.state
    from Person p LEFT JOIN Address q
    on p.personId=q.personId ;
