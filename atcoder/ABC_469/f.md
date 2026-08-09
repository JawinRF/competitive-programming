# ABC 469 F — GCD Maximum Spanning Tree

## Problem

You are given a sequence of positive integers `A` of length `N`.

There is a weighted **complete** undirected graph on `N` vertices numbered
`1 … N`. For every `1 ≤ i < j ≤ N`, the weight of the edge joining `i` and `j`
is `gcd(A_i, A_j)`.

Find the **maximum** possible sum of edge weights of a spanning tree of this
graph.

### Constraints

- `2 ≤ N ≤ 2×10^5`
- `1 ≤ A_1 < A_2 < ⋯ < A_N ≤ 10^6` (so the values are **distinct and sorted**)
- All input values are integers.

Worth noting: the values are given already sorted and distinct — that is what
makes a presence array indexed by value legal.

---

## Session log

### Hint 1 — stop thinking in edges

There are `~N²/2 = 2×10^10` edges, so they can never be materialised. But every
edge weight is a `gcd` of two values `≤ 10^6`, so there are only `10^6` distinct
**weights**.

Flip the loop. Instead of *"for each edge, what is its weight?"*, ask:

> for a fixed value `g`, which vertices have an edge of weight at least `g`
> between them?

Then ask which classic MST algorithm lets you consume edges grouped by weight.

### My reading of it

> I can't enumerate the ~N²/2 edges, but I can enumerate the possible weights
> i.e every edge weight is at most 10^6
>
> m = 1e6 then
> for each weight w . iterations m/w
> which is O(m log m) complexity

That is the whole idea. Filling in the two halves:

- **Which vertices?** `gcd(A_i, A_j) ≥ g` exactly when `g` divides both. So the
  group for `g` is *the values present in `A` that are multiples of `g`* — found
  by striding `g, 2g, 3g, …` over a presence array. Summed over all `g` that is
  `Σ m/g = O(m log m) ≈ 1.4×10^7` steps.
- **Which algorithm?** **Kruskal, descending.** Process `g` from `10^6` down to
  `1` and union the whole group together with a DSU, paying `g` for each union
  that actually merges two components.

Why paying `g` per merge is correct even though the group's true edge weights
are only `≥ g`: any pair in the group whose gcd is `> g` was already processed
at its own, larger weight, so those pairs are already contracted into one DSU
node by the time `g` comes up. Every union that still succeeds at `g` is a pair
whose best available connection really is `g`. And within the group the merge
*shape* is irrelevant — a star through `collections[0]` costs exactly as much as
any other spanning structure, so no explicit edge list is needed.

**Complexity:** `O(m log m)` for the sieve-style striding, plus `Σ_{x∈A} d(x)`
DSU operations.

### Hint 2 — reviewing the first version

The algorithm was right; one line was an optimisation bolted on top of it:

```cpp
mark[collections[i]] = 0 ;   // after a successful union
```

The question to ask: after unmarking a value, its component is represented — for
every future, smaller `k` — only by the values still marked. Is `collections[0]`
guaranteed to be divisible by everything the value you just erased was divisible
by?

### My response

> when i commented out i realise it worked .
>
> The thing is 3 5 15 is such example . The thing is without the comment also its
> still fast because the still its harmonic

Both halves correct, and the second half is the reason the optimisation was
never worth having: the `j += k` loop walks all `m/k` slots regardless of what is
marked, so unmarking skipped a `push_back`, never an iteration. The harmonic
bound was already free.

---

## Bugs found in the first version

| # | Line | Bug |
|---|---|---|
| 1 | `mark[collections[i]] = 0 ;` | Unmarking a value after merging it removes it from **all** later groups. Its component then looks like it contains no multiple of some smaller `k`, and that component gets connected at a lower weight than it deserves. |

### The disappearing representative

`A = 3 5 15`. True answer `gcd(3,15) + gcd(5,15) = 3 + 5 = 8`.

| `k` | group | action |
|---|---|---|
| 15 | `{15}` | size 1, nothing |
| 5 | `{5, 15}` | union, `wSum = 5`, then **unmark 15** |
| 3 | `{3}` | 15 is gone → nothing |
| 1 | `{3, 5}` | union, `wSum = 6` ❌ |

`15` was the only member of its component divisible by `3`; erasing it cost the
weight-3 edge and bought a weight-1 edge instead. The surviving representative
`collections[0] = 5` carries none of `15`'s divisors.

**General lesson:** a DSU component is only "reachable" through the members you
can still enumerate. Deleting a member is safe only if the one you keep dominates
its whole divisor set — which the smallest multiple of `k` does not.

### Non-bug: the `log n` in `getIdx`

`getIdx` does a `lower_bound` inside the main loop, which looks like it multiplies
`1.4×10^7` by `log n`. It does not — the binary search only runs for values
actually present, and that total is `Σ_{x∈A} d(x)`, a few million for `2×10^5`
values under `10^6`. Harmless, left alone.

---

## Takeaway pattern

**Complete graph with number-theoretic weights ⇒ Kruskal over weight classes.**

1. The edge set is too big to build, but the weight *range* is small.
2. For a weight `g`, characterise the set of vertices joined by an edge of weight
   `≥ g` — for `gcd` weights that is "the multiples of `g` present", reachable by
   sieve striding in `O(m/g)`.
3. Sweep `g` downward (upward for a minimum spanning tree) and union each whole
   group with a DSU, charging `g` per successful merge. Edges heavier than `g`
   are already contracted, so the charge is exact.
4. Never delete elements from the presence array — later, smaller `g` still need
   them as divisor witnesses.

---

## Implementation

Source: [`f.cpp`](f.cpp#L87-L115) &nbsp;·&nbsp; `O(m log m + Σ d(A_i) · α)` with `m = 10^6`

<details open>
<summary><b>solve()</b> — click to collapse</summary>

```cpp
void solve(){
    int n ;  cin >> n ;
    vector<int> a(n) ; read(a) ;
    vector<bool> mark(m+1,0) ;
    for(int x:a){
        mark[x] = 1 ;
    }
    DSU d(n);
    sort(all(a)) ; 
    auto getIdx = [&](int x){
        return lower_bound(all(a),x) - a.begin() ;
    };
    int wSum = 0 ;
    for(int k = m  ; k>=1 ; --k){
        vector<int> collections ; 
        for(int j = k ; j<=m ; j+=k){
            if(mark[j]){
                collections.pb(j) ;
            }
        }
        for(int i = 1 ; i<(int)collections.size() ; ++i){
            if(d.unite(getIdx(collections[i]),getIdx(collections[0]))){
                wSum += k ;
                // mark[collections[i]] = 0 ; 
            }
        }
    }
    put(wSum) ;
}
```

</details>

### Phase map

Line numbers refer to [`f.cpp`](f.cpp); click any range to open it on GitHub
(relative links resolve on GitHub, not in the VSCode markdown preview).

| Lines | Phase | What it does | Cost |
|---|---|---|---|
| [88–93](f.cpp#L88-L93) | **Presence array** | `mark[v] = 1` if value `v` occurs. Legal because the `A_i` are distinct and `≤ 10^6` | `O(N + m)` |
| [94–98](f.cpp#L94-L98) | **DSU + value→index** | One DSU node per vertex; `getIdx` maps a value back to its vertex | `O(N log N)` |
| [100](f.cpp#L100) | **Descending weight sweep** | Kruskal without an edge list: consider weight classes from `10^6` down to `1` | `m` steps |
| [101–106](f.cpp#L101-L106) | **Group for `k`** | Stride `k, 2k, 3k, …` to collect present multiples — exactly the vertices joined by an edge of weight `≥ k` | `O(m/k)`, `O(m log m)` total |
| [107–112](f.cpp#L107-L112) | **Star merge** | Union every member into `collections[0]`, adding `k` per real merge. Pairs with gcd `> k` are already contracted, so each success is genuinely worth `k` | `Σ d(A_i)` |
| [110](f.cpp#L110) | ⚠️ **Do not unmark** | Erasing a merged value strips its component of divisor witnesses for smaller `k` — fails on `3 5 15` (`6` instead of `8`) | — |
| [114](f.cpp#L114) | **Answer** | `wSum` accumulates `N-1` merges by construction | — |
