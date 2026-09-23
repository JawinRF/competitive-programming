SELECT user_id as buyer_id,join_date,count(buyer_id) as orders_in_2019 FROM
(
    SELECT * FROM 
    USERS AS u
    LEFT JOIN 
    (
        SELECT buyer_id FROM orders
        WHERE orders.order_date>='2019-01-01' AND 
        orders.order_date<='2019-12-31'
    )as o
    ON u.user_id = o.buyer_id
) AS N
GROUP BY user_id ; 