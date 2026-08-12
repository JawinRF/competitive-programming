# TODO — Minimum Cost to Merge Stones (LeetCode 1000)

Solved and committed. `code.cpp` is O(n^3 * k^2), `codeV2.cpp` is O(n^3 * k).
These are the follow-ups I left for later.

## 1. Drop the pile-count dimension — O(n^3 / k)

The third state is not free information. It is forced by the length.
For an interval of length `L`, every reachable pile count `p` obeys
`p == L (mod k-1)`.

So use a 2D table:

```
dp[i][j] = min cost to merge [i..j] into as few piles as possible
           (that count is (L-1) % (k-1) + 1)
```

- Split: `dp[i][j] = min over mid of dp[i][mid] + dp[mid+1][j]`, where `mid`
  steps by `k-1` starting at `i`. That step is exactly the condition for the
  left block to collapse to 1 pile.
- Then, if `(L-1) % (k-1) == 0`, add `sum(i..j)`. Same rule as the 3D version.

Two dimensions, and the split loop shrinks by a factor of `k-1`.

## 2. The k == 2 case

With `k == 2` this becomes the optimal merge problem on a **fixed** order
(no reordering, unlike Huffman). Two known speed-ups to study:

1. **Knuth optimization — O(n^2).**
   The cost obeys the quadrangle inequality, so the best split point is
   monotone: `opt[i][j-1] <= opt[i][j] <= opt[i+1][j]`. Loop `mid` inside
   those bounds instead of over the whole interval. Check the proof of the
   quadrangle inequality here, not only the recipe.

2. **Garsia–Wachs — O(n log n).**
   Solves the optimal alphabetic binary tree. Read why the "find the first
   pair with `w[i] <= w[i+2]`, merge, move left" step keeps the order valid.

Neither extends to general `k`.

## Notes

- At `n <= 30` none of this changes the verdict. Item 1 is the reusable idea:
  a DP dimension that is determined by the others can be deleted.
- Header comment in `codeV2.cpp` still says the split loop tries every left
  count. That is stale — V2 already fixes it to 1.
