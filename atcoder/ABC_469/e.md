# ABC 469 E

## Problem

You are given a string `S` of length `N` consisting of `o` and `x`. It is
guaranteed that `S` contains at least `K` occurrences of `o`.

Takahashi played a game `N` times. In the `i`-th game he won if `S[i] == 'o'`
and lost if `S[i] == 'x'`.

Choose a pair `(l, r)` with:

- `1 ≤ l ≤ r ≤ N`
- he won at least `K` times among games `l … r`

Find the **maximum possible win rate** over games `l … r`.

### Constraints

- `1 ≤ K ≤ N ≤ 10^6`
- `S` is a string of length `N` of `o` / `x`, containing at least `K` `o`s
- Absolute or relative error up to `10^-6` accepted

### Samples

| N K | S | Answer |
|---|---|---|
| `10 4` | `oxooxoxxox` | `0.6666666666` |
| `5 1` | `xxoxx` | `1` |
| `16 10` | `xxxoxooooxoxoooo` | `0.769230769230769` |

---

## Session log

### My starting thoughts

> I thought of binary searching but couldn't come up with a way to quickly
> verify. Terminating when range length < `10^-6`.
>
> I also thought that the start and end must always be a hit, else we can shrink
> and increase the score.
>
> Thought of prefix sums, for maximising `(cnt[r] - cnt[l-1]) / (r - (l-1))`,
> but couldn't come up with a way to show any monotonicity or a way to make
> transitions cheap.

### Hint 1 — what the constraint looks like geometrically

Fix a candidate ratio and rewrite the goal as *"does some valid interval have a
nonnegative sum?"* with the right per-character weights — prefix sums already
give that. The only thing blocking the usual max-subarray scan is the `≥ K` wins
constraint, so ask:

- **For a fixed `r`, exactly which `l` are allowed?** Describe that set as a
  *shape*, not a list.
- What happens to that shape as `r` increases by one?

### My response

> For a fixed `l` the constraint becomes `cnt[r] >= K - cnt[l-1]`. Since `cnt` is
> monotonic we can binary search for such an `r` — but what next?

### Hint 2 — two follow-ups

1. No binary search per `l` is needed: as `l` increases, the threshold `r₀(l)`
   only moves one way. Monotone ⇒ a single pointer over the array.
2. The allowed `r` for a fixed `l` form a **suffix** `[r₀(l), N]`. The check
   needs the max of the weighted prefix `P[r]` over that suffix. What single
   precomputed array turns *"max over a suffix"* into an `O(1)` lookup?

### My response

> `cnt[r] - cnt[l-1] >= K`, so `cnt[r] >= K + cnt[l-1]`, and since `cnt`
> increases by only one at a time we can have a vector `mpp` with values from
> `0` to `N` that stores it.

Correct — `mpp[v]` = the first index where the o-count reaches `v`, so
`r₀(l) = mpp[K + cnt[l-1]]` in `O(1)`, no binary search. Same effect as the
monotone pointer, just tabulated.

### Hint 3 — killing the division

Don't maximise the ratio directly. Binary search `x` and ask *"is ratio ≥ `x`
possible?"*, then multiply out the denominator:

```
cnt[r] - cnt[l-1] >= x * (r - l + 1)
```

Push everything with `r` to one side and everything with `l-1` to the other.
Both sides become the **same function of a single index** — that function is
`P[·]`, and the question collapses to *"is `P[r] - P[l-1] >= 0` for some allowed
pair?"*

### My derivation

```
cnt[r] - cnt[l-1] >= x*r - x*l + x
cnt[r] - x*r      >= cnt[l-1] - x*(l-1)
```

Both sides are the same function, so define `P[i] = cnt[i] - x*i` in `O(n)`.
Full plan:

1. For each `l`, find the minimum `r` satisfying `cnt[r] >= K + cnt[l-1]`.
2. Binary search for `x`.
3. Inside the binary-search loop, compute `P[i]`.
4. Compute the suffix max of `P`.
5. Iterate and check whether `suffmax[minR[l]] >= P[l-1]`; if so, record the
   answer and probe higher.

Correct. `minR` / `mpp` is built **once outside** the loop (it does not depend on
`x`). Inside, `P` and `suffmax` are `O(N)` each; ~50 bisection steps on `[0, 1]`
lands well under `10^-6`.

**Complexity:** `O(N log(1/ε))`.

---

## Bugs found in the first version

| # | Line | Bug |
|---|---|---|
| 1 | `if(s[i]=='0')` | Compared against the **digit** `'0'` instead of the letter `'o'` — every count came out `0`. |
| 2 | `cin >> tc;` | This problem has no test-case count; it swallowed `N` and ran `solve()` `N` times on garbage. |
| 3 | `suffmax[minR] - P[i]` | Off-by-one: with `i` as the left endpoint `l`, the left-hand term must be `P[l-1]`, not `P[i]`. |
| 4 | `if(minR > i ...)` | Should be `minR >= i`. As written it forbids `l == r`, which is exactly sample 2's answer (`xxoxx`, `K=1` → printed `0.5` instead of `1`). |

### The `l = 0` sentinel

There is no `P[-1]` in the array. Plugging `l-1 = -1` into the formula
`cnt[l-1] - x*(l-1)` gives `0 - x*(-1)` = **`x`**, not `0`:

```cpp
double lv = (i > 0 ? P[i-1] : x);
if (suffmax[minR] - lv >= 0) { ok = true; break; }
```

**General lesson:** the empty-prefix sentinel is *not* zero — it is whatever the
`P` formula evaluates to at index `-1`. When binary-searching a ratio like this,
it is usually cleaner to make `cnt` and `P` **1-indexed of size `n+1` with
`P[0] = 0`**, so the sentinel falls out for free instead of needing a special
case.

---

## Verification of the fixed code

- **Samples 1–3:** `0.6666666`, `0.9999999`, `0.7692307` — all within `10^-6`.
- **Stress test:** 400 random cases (`n ≤ 14`, varied o-density, random `K`)
  against an `O(n²)` brute force — no mismatches.
- **Timing at `n = 10^6`:** 0.16 s random, 0.14 s on `o…ox…x`, 0.17 s on
  `x…xo` — comfortably inside the limit even with the per-iteration vector
  allocations, so leaving them is fine.

## Takeaway pattern

**Maximise a ratio ⇒ binary search the ratio + reduce to a sign test.**

1. Guess `x`, rewrite `A/B ≥ x` as `A - x·B ≥ 0` to remove the division.
2. Get both sides into the same function of one index → `P[i]`.
3. Handle the side constraint by asking *which left/right endpoints are legal*
   and describing that set as a prefix/suffix.
4. Prefix/suffix extremum array turns the pairing into `O(1)` per endpoint.

---

## Implementation

Source: [`e.cpp`](e.cpp#L44-L102) &nbsp;·&nbsp; `O(N log(1/ε))` &nbsp;·&nbsp; ~0.17 s at `N = 10^6`

<details open>
<summary><b>solve()</b> — click to collapse</summary>

```cpp
void solve(){
    int n , k ; cin >> n >> k ;  
    string s ; cin >> s ;  
    vector<int> cnt(n)  ;  
    for(int i = 0 ; i<n ; ++i){
        if(s[i]=='o')cnt[i] = 1 ;  
        if(i>0)cnt[i]+=cnt[i-1] ;
    }
    vector<int> mpp(n+1,INF) ;   
    // cnt[r] - cnt[l-1] >= k  
    for(int i = 0 ; i<n ; ++i){
        mpp[cnt[i]] = min(mpp[cnt[i]],i) ;
    }

    double ans = 0 , l = 0 , h = 1 ;  
    while(h-l>1e-7){
        double x = (l+h)/2 ;  

        bool ok = false ; 

        vector<double> P(n) ;  
        for(int i = 0 ; i<n ; ++i){
            P[i] = cnt[i] - x*i ; 
        }
        vector<double> suffmax(n,-INF) ;
        suffmax[n-1] = P[n-1] ;
        for(int i = n-2 ; i>=0 ; --i){
            suffmax[i] = max(suffmax[i+1],P[i]) ;
        }

        for(int i = 0; i<n ; ++i){
            // cnt[r] - cnt[l-1] >= k
            // cnt[r] >= k + cnt[l-1]
            int val = k + (i>0?cnt[i-1]:0) ;
            if(val<=n){
                int minR = mpp[val] ; 
                if(minR>=i && minR<n){
                    // cnt[r] - x*r >= cnt[l-1] - x*(l-1)
                    // if l = 0  
                    // cnt[-1] = 0 as no entty and - x*(l-1) becomes -x*(-1) = x
                    if(suffmax[minR]-(i>0?P[i-1]:x)>=0){
                        ok = true ; 
                        break ; 
                    }
                }
            }
        }
        if(ok){
            ans = x ; 
            l = x ;
        }
        else{
            h = x ;
        }
    }
    
    sputdouble(ans) ;
    cout<<"\n" ; 
}
```

</details>

### Phase map

Line numbers refer to [`e.cpp`](e.cpp); click any range to open it on GitHub.

| Lines | Phase | What it does | Cost |
|---|---|---|---|
| [45–51](e.cpp#L45-L51) | **Prefix counts** | `cnt[i]` = number of `o` in `S[0..i]` | `O(N)` |
| [52–56](e.cpp#L52-L56) | **`mpp` table** | `mpp[v]` = first index where the o-count reaches `v`. Built **once**, outside the search — it does not depend on `x` | `O(N)` |
| [58–60](e.cpp#L58-L60) | **Bisect the answer** | Guess a rate `x ∈ [0,1]`; ~24 iterations to reach `10^-7` | `log(1/ε)` |
| [64–67](e.cpp#L64-L67) | **Weighted prefix** | `P[i] = cnt[i] - x·i`, the same function on both sides of the rearranged inequality | `O(N)` |
| [68–72](e.cpp#L68-L72) | **Suffix max** | Turns *"best `r` in `[r₀, N)`"* into an `O(1)` lookup | `O(N)` |
| [74–90](e.cpp#L74-L90) | **Feasibility scan** | For each `l`: jump to `r₀ = mpp[K + cnt[l-1]]`, then test `suffmax[r₀] ≥ P[l-1]` | `O(N)` |
| [84](e.cpp#L84) | ⚠️ **Sentinel** | `l = 0` has no `P[-1]`; the formula evaluates to `x`, not `0` | — |
| [91–97](e.cpp#L91-L97) | **Narrow** | Feasible → record and probe higher; else lower the ceiling | — |


