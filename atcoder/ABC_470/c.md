# ABC 470 C

## Problem

There is a length-`N` integer sequence `A = (A_1, A_2, …, A_N)`. Initially every
element is `0`. Process `Q` queries in order, each of one of two types:

- `1 x` — increase `A_x` by 1.
- `2` — for each `i = 1 … N`, if `A_i ≥ 1`, decrease `A_i` by 1.

Print the bitwise XOR of `A_1, A_2, …, A_N` immediately after each query.

### Constraints

- `1 ≤ N ≤ 5×10^5`
- `1 ≤ Q ≤ 5×10^5`
- `1 ≤ x ≤ N`
- All input values are integers.

---

## Session log

### My starting thoughts

> I think for this bruteforce will work.
>
> I mean consider only disticnt values after some operations and their xor be x
> if some value y gets operation 1
> `x -> x^y^(y+1)`
>
> now operation 2 is fine
> because the difficult case is when there are too many distinct numbers
> let it be `l`
> now to reduce `l` by only operation 2 until its size goes to zero
> we ne `~l^2` operations and each operation `l` so `~l^3` operations
>
> to reach a lenght `l` we need `l^2` operation 1 operations
> with `Q` given we can have length to be `sqrt(Q)`
> then operations of type 2 `l^3 = sqrt(Q)^3` which is `3.5355×10^8`

The incremental-XOR idea is exactly right. The complexity estimate is the part
worth re-examining.

### Hint 1 — where the estimate leaks

Don't bound a type-2 op by "worst case `l`" and multiply by `Q`. Ask instead:
when a type-2 op makes you walk `l` distinct values, how much total sum does that
same op destroy? Then compare against how much sum can ever be created.

### The counting argument

Take the potential `Φ = A_1 + A_2 + … + A_N`. It starts at `0` and is always `≥ 0`.

- Only type-1 ops increase `Φ`, by exactly `1` each — so total growth `≤ Q`.
- A type-2 op with `k` nonzero elements walks `k` elements (the cost) and each
  loses exactly `1`, so `Φ` drops by exactly `k`.

Since `Φ ≥ 0` always, total decrease `≤` total increase `≤ Q`:

```
sum over type-2 ops of k_j  =  total decrease of Φ  ≤  Q
```

The left side *is* the total work across all type-2 ops. So the whole simulation
is `O(N + Q)`.

**Why `l^3` overcounted:** the two factors are mutually exclusive. An op that
touches many elements is an op that burns a lot of sum, and the sum budget is only
`Q`. Reaching `l = 1000` costs ~`5×10^5` increments — after paying that there are
no queries left to actually *perform* `5×10^5` expensive type-2 ops. The potential
charges each unit of work to a unit of sum that was paid for earlier, so no
adversarial sequence beats it.

### Consequence

No distinct-value grouping and no offset are needed. Literal simulation over the
nonzero elements is already linear.

---

## Bugs found on the way

| # | Line | Bug |
|---|---|---|
| 1 | `cin >> tc` in `main` | Template default. This problem has no test-case count, so that read swallowed `N`. |
| 2 | `vector<int> a(n) ; read(a) ;` | The input has no array — `A` starts all zeros. The loop consumed `N` integers off the *query* stream, then hit EOF and left `cin` in a fail state, making every later `>>` a no-op. |
| 3 | type-2 branch, `a[k-1] += v` | Mixed a `freq` map (value → count) with `a` (per-element values) — two different index spaces. |
| 4 | type-2 branch | Never updated `Xor` at all. |

### The trap: a `freq` map alone cannot work

A type-1 query needs the **current** value of element `i`. After type-2 ops have
run, `a[i]` is stale, and no global offset repairs it — elements stop decreasing
at different times, so one that hit `0` early has absorbed fewer decrements than
one that stayed positive. `freq` knows *how many* elements sit at each value, but
not *which*, so it can never answer `a[i]`.

The only way to keep `a[i]` honest is to walk the nonzero elements on each type-2
op — and once you're doing that, the map is redundant.

**General lesson:** a "lazy global shift" only works when every element receives
the shift. A clamp at `0` breaks that uniformity, and then you must touch elements
individually — check whether the amortized bound makes that free before inventing
machinery to avoid it.

---

## Takeaway pattern

**Charge the work to the potential.**

1. Find a quantity `Φ` that is non-negative and only grows via cheap ops.
2. Show the expensive op's cost equals the amount it decreases `Φ`.
3. Then total expensive work `≤` total growth of `Φ`, regardless of the order of ops.

This turns "each op is `O(√Q)`, so `O(Q√Q)` total" into `O(Q)` whenever the
expensive op consumes what the cheap op produced.

---

## Implementation

Source: [`c.cpp`](c.cpp#L59-L87) &nbsp;·&nbsp; `O(N + Q)`

<details open>
<summary><b>solve()</b> — click to collapse</summary>

```cpp
void solve(){
    int n , q ; cin >> n >> q ;  
    vector<int> a(n, 0) ;
    vector<int> nz ;
    int Xor = 0 ; 
    while(q--){
        int ty ; cin >> ty ;  
        if(ty==1){
            int i ; cin >> i ; --i ;  
            
            int x = a[i] ;a[i]++ ; 
            Xor = Xor ^ x ^ (x+1) ; 
            if(x==0){
                nz.pb(i) ; 
            }
        }
        else{
            int w_ptr = 0 ;
            for(int idx : nz){
                Xor = Xor ^ a[idx] ^ (a[idx]-1) ;
                if(--a[idx] > 0){
                    nz[w_ptr++] = idx ;
                }
            }
            nz.resize(w_ptr) ;
        }
        put(Xor) ;
    }
}
```

</details>

### Phase map

Line numbers refer to [`c.cpp`](c.cpp); the ranges are clickable on GitHub (they
do not resolve in the VSCode markdown preview).

| Lines | Phase | What it does | Cost |
|---|---|---|---|
| [60–63](c.cpp#L60-L63) | **Setup** | `a` holds every element's value, `nz` holds only the indices currently nonzero, `Xor` is maintained incrementally rather than recomputed. | `O(N)` |
| [66–74](c.cpp#L66-L74) | **Type 1** | Read old value `x`, bump it, fold the change into the running XOR with `Xor ^= x ^ (x+1)`. | `O(1)` |
| [71–73](c.cpp#L71-L73) | ⚠️ **`0 → 1` edge only** | Push into `nz` **only** when the old value was `0`. Pushing on every type-1 query would let one index sit in `nz` twice and get decremented twice in a single sweep. | — |
| [75–84](c.cpp#L75-L84) | **Type 2 sweep** | Walk `nz`, fold `a[idx] → a[idx]-1` into the XOR, and compact survivors forward with a write pointer so the ones that reached `0` are dropped in `O(1)` each. | `O(k)`, `O(Q)` total |
| [85](c.cpp#L85) | **Output** | `Xor` is already correct — it was updated in place, never rebuilt. | `O(1)` |
