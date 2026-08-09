# ABC 469 D

## Problem

There is a game with `N` players `1, 2, …, N`. The game is played in a format
where two players face each other one-on-one.

A tournament among the `N` players was held `M` times, and the two players `A_m`
and `B_m` advanced to the final of the `m`-th tournament.

Find the number of pairs of integers `x` and `y` satisfying:

- `1 ≤ x < y ≤ N`
- In **every** tournament, at least one of players `x` and `y` advanced to the final.

### Constraints

- `2 ≤ N ≤ 2×10^5`
- `1 ≤ M ≤ 2×10^5`
- `1 ≤ A_i < B_i ≤ N`
- All input values are integers.

## Approach

Model each tournament as an edge `{A_m, B_m}`. A pair `(x, y)` is valid iff every
edge is incident to `x` or `y` — that is, `{x, y}` is a **vertex cover of size 2**
of the multigraph. So the task is to count all size-2 vertex covers.

The standard branching trick:

1. Edge `0` must be covered, so one member of the pair is `A[0]` or `B[0]`.
   That gives only two candidates for the first member.
2. Fix that member `v`. Every edge that `v` misses must be covered by the
   partner `u`, so `u` lies in the intersection of all missed edges.
   Intersecting with just the **first** missed edge already narrows `u` to two
   candidates, `A[j]` and `B[j]` — the remaining missed edges only filter.
3. Verify each of those two candidates against all `M` edges in `O(M)`.
   The verification can reject both: e.g. edges `(1,2), (3,4), (3,5), (4,5)`
   answer `0`, since the triangle `3-4-5` needs two vertices on its own.
4. Degenerate case: if `v` misses no edge, it covers everything alone and the
   partner is unconstrained → all `N-1` pairs `{v, u}` are valid. This can fire
   for both `A[0]` and `B[0]` at once (when every tournament is the same pair),
   which is why the results are deduplicated at the end.

Branch → narrow to 2 → verify, done twice, so the whole thing is a constant
number of `O(M)` scans.

**Complexity:** `O(N + M)` work, `O(N log N)` overall from the final dedupe sort.
