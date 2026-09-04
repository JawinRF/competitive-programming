# LeetCode 956: Tallest Billboard

## Idea

Each rod has three choices:

1. Put it on the left support.
2. Put it on the right support.
3. Do not use it.

Trying every assignment directly takes `O(3^n)` time. Meet in the middle reduces the number of generated assignments to about `O(3^(n/2))` per half.

For one assignment, define its signed difference as:

```text
difference = left support sum - right support sum
```

For a fixed difference, only the largest possible left support sum matters. If two assignments have the same difference, adding the same remaining rods to both supports affects their final heights in the same way. Therefore, store:

```text
best[difference] = maximum left support sum among assignments with this difference
```

The total rod sum is at most `5000`, so every difference lies in `[-5000, 5000]`. The two halves form equal supports when their differences cancel. If the first half has difference `d`, the second half must have difference `-d`.

For every difference in the first half, look up the opposite difference in the second half and maximize the sum of the two stored left support heights.

## Proof

Consider any valid solution that uses assignments from the first and second halves.

1. Let their signed differences be `d1` and `d2`. Equal final supports require `d1 + d2 = 0`, so `d2 = -d1`.
2. For a fixed difference in either half, replacing an assignment with another assignment having the same difference and a larger left support sum preserves the possibility of equal final supports and cannot reduce the answer.
3. Therefore, `best1[d1] + best2[-d1]` is the best solution whose two halves have difference `d1` and `-d1`.
4. The merge checks every difference stored in the first half, so it checks the pair of differences for every valid solution.

Thus, the maximum value found by the merge is the tallest possible equal support.

## Complexity

Each rod has three choices in each half, so state generation takes `O(3^(n/2))` time. The merge takes `O(S)` expected time, where `S` is the total sum of all rods, because all differences lie in `[-S, S]`.

The total expected time complexity is:

```text
O(3^(n/2) + S)
```

The space complexity is `O(3^(n/2))` in the worst case.
