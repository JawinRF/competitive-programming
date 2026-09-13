The total score is always the number of potatoes, so the game is zero-sum. Consider a maximal run of 1s followed by an empty position. Every potato except the last one is blocked.

If the last potato is passed early, it reaches the other team and leaves an empty position behind, enabling the preceding potato. Both newly enabled players belong to the other team, so with another round the opponent may cancel the point just gained. Passing in the final round leaves no response and is optimal.

Thus only the last potato of each run moves one step in the final round. If the whole cycle is filled, nothing moves. For an initial position :

- if  and , the team owning  scores;
- if , the other team scores.

Scan the cycle once. The implementation is zero-indexed: even positions belong to red and odd positions to blue. Their scores are printed in that order.

The time complexity is  and the space complexity is .
