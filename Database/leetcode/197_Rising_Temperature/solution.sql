SELECT W.id
FROM Weather AS W
INNER JOIN
(
    SELECT recordDate, temperature
    FROM Weather
) AS T
ON W.recordDate = T.recordDate + INTERVAL 1 DAY
AND W.temperature > T.temperature;
