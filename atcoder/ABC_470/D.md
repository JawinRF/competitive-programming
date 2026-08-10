# ABC 470 D

## Problem

You are given a permutation `P = (P_1, …, P_N)` of `(1, …, N)`. Process `Q`
queries in order:

- `1 x y` — swap the values of `P_x` and `P_y`.
- `2` — replace `P` by the unique permutation `P'` satisfying
  `P[P'[i]] = i` for every `1 ≤ i ≤ N`.

Output `P_1, …, P_N` after all queries.

`P[P'[i]] = i` says exactly that `P'` is the **inverse** of `P`: `P' = P^-1`.

### Constraints

- `2 ≤ N ≤ 5×10^5`
- `1 ≤ Q ≤ 5×10^5`
- `1 ≤ x < y ≤ N` for type-1 queries
- All input values are integers.

### Samples

| N Q | P | Queries | Answer |
|---|---|---|---|
| `5 5` | `2 1 3 5 4` | `1 2 4`, `2`, `1 2 3`, `1 3 4`, `2` | `4 5 2 1 3` |

---

## Session log

### The first confusion — the debug print

A scratch loop applied the inverse six times and printed each result:

> This outputs 5
> ```
> 2 1 3 5 4
> 2 1 3 5 4
> 2 1 3 5 4
> 2 1 3 5 4
> 2 1 3 5 4
> 2 1 3 5 4
> 2 1 3 5 4
> ```
> which is wierd

Not weird — two things stacked on top of each other:

1. `f` returns `a^-1`, and inverting twice returns the original. So the print can
   only ever alternate between two lines, never show six different ones.
2. The test input `2 1 3 5 4` is an **involution**: `1↔2`, `3` fixed, `4↔5`. All
   cycles have length 1 or 2, so `P^-1 = P` and both lines of the alternation are
   the same.

A non-involution such as `2 3 1` makes the period visible: `3 1 2`, `2 3 1`,
`3 1 2`, …

### The key idea

Never actually rebuild the permutation on a type-2 query. Keep **both**
directions at all times:

```
a = the permutation
b = a^-1
```

A type-2 query then costs `O(1)` — it just swaps which of the two is "the real
`P`". Track that with one bit, `curr`.

### The WA

> forget O(N^2) why this doesntr work

The type-1 branch always swapped inside `a`:

```cpp
b[a[x]] = y ;
b[a[y]] = x ;
swap(a[x],a[y]) ;
```

That is correct **only while `curr == 0`**. After a type-2 query the real
permutation is `b`, so `1 x y` must swap `b[x]` and `b[y]`, and patch `a`.

Trace of sample 1 with the broken version:

```
a = 2 1 3 5 4 , b = 2 1 3 5 4
Q1  1 2 4  →  b = 4 1 3 5 2 , a = 2 5 3 1 4     P = a  ✓
Q2  2      →  curr = 1                          P = b = 4 1 3 5 2  ✓
Q3  1 2 3  →  swaps a[2],a[3]  ← wrong array
              a = 2 3 5 1 4 , b = 4 1 2 5 3     P = b = 4 1 2 5 3  ✗ (want 4 3 1 5 2)
Q4  1 3 4  →  a = 2 3 1 5 4 , b = 3 1 2 5 4
Q5  2      →  curr = 0, print a = 2 3 1 5 4     want 4 5 2 1 3
```

It fails on sample 1 itself.

### The fix

> i just realsied this . Ig its correct

Branch on `curr` and apply the same three lines with `a` and `b` exchanged — the
relation is symmetric, `a = b^-1` holds just as well as `b = a^-1`.

**Complexity:** `O(N + Q)`.

---

## Bugs found on the way

| # | Line | Bug |
|---|---|---|
| 1 | `for j … if(a[j]==i)` in `f` | `O(N^2)` inverse. At `N = 5×10^5` this TLEs before a single query runs. Replaced by the direct `mpp[a[i]] = i`. |
| 2 | `swap(a[x],a[y])` unconditionally | Type-1 always wrote to `a`, ignoring `curr`. Every swap after the first type-2 query landed in the wrong array. Fails sample 1. |

### The trap: the swap must follow the flag

`curr` does not only pick what to *print*. It picks what the queries *act on*.
A lazy-flag trick is only sound when every operation between flips respects the
flag:

```cpp
if(curr==0){ b[a[x]] = y ; b[a[y]] = x ; swap(a[x],a[y]) ; }
else       { a[b[x]] = y ; a[b[y]] = x ; swap(b[x],b[y]) ; }
```

Note also the ordering inside a branch: patch the inverse **before** the swap.
`b[a[x]]` needs the old `a[x]`, so swapping first destroys the index it needs.

**General lesson:** when a flag means "the roles of two structures are
exchanged", every write path needs the branch, not just the read path.

---

## Takeaway pattern

**Make the expensive involution a flag.**

1. The operation you cannot afford is its own inverse (invert, reverse, negate,
   transpose).
2. Store both images at once and keep them in sync.
3. The expensive operation becomes `curr ^= 1`, `O(1)`.
4. Every other operation now has two branches — one per value of the flag —
   which is where the bug lives.

---

## Implementation

Source: [`D.cpp`](D.cpp#L76-L112) &nbsp;·&nbsp; `O(N + Q)`

<details open>
<summary><b>solve()</b> — click to collapse</summary>

```cpp
void solve(){
    int n ,q  ; cin >> n >> q ;
    vector<int> a(n+1) ; readOff(a,1,n) ;
    // for(int i = 1 ; i<=6 ; ++i){
    //     a = f(n,a) ;
    //     show(a,1) ;
    // }
    vector<int> b = f(n,a) ;

    int curr = 0  ;
    while(q--){
        int t  ; cin >> t ;
        if(t==1){
            int x , y ; cin >> x >> y ;
            if(curr==0){
                b[a[x]] = y ;
                b[a[y]] = x ;
                swap(a[x],a[y]) ;
            }
            else{
                a[b[x]] = y ;
                a[b[y]] = x ;
                swap(b[x],b[y]) ;
            }
            
        }
        else{
            curr ^= 1 ;
        }
    }
    if(curr==0){
        show(a,1) ;
    }
    else{
        show(b,1) ;
    }
}
```

</details>

### Phase map

Line numbers refer to [`D.cpp`](D.cpp); the ranges are clickable on GitHub (they
do not resolve in the VSCode markdown preview).

| Lines | Phase | What it does | Cost |
|---|---|---|---|
| [59–75](D.cpp#L59-L75) | **Inverse** | `mpp[a[i]] = i` builds `a^-1` in one pass. (`mpp` and `res` are the same array — the copy loop is redundant.) | `O(N)` |
| [77–83](D.cpp#L77-L83) | **Setup** | Read `a`, build `b = a^-1`. Both directions are live from here on. | `O(N)` |
| [85](D.cpp#L85) | **The flag** | `curr = 0` means the real `P` is `a`; `curr = 1` means it is `b`. | — |
| [90–99](D.cpp#L90-L99) | **Type 1** | Swap the two values in the active array, and patch the two matching entries of the other one. | `O(1)` |
| [90](D.cpp#L90) | ⚠️ **Branch on `curr`** | Without this test every swap after the first type-2 query hits the wrong array. Fails sample 1. | — |
| [91–93](D.cpp#L91-L93) | ⚠️ **Patch before swap** | `b[a[x]]` reads the *old* `a[x]`. Move `swap` above these two lines and the indices are already gone. | — |
| [102–104](D.cpp#L102-L104) | **Type 2** | Flip the flag. No permutation is rebuilt. | `O(1)` |
| [106–111](D.cpp#L106-L111) | **Output** | Print whichever array the flag currently names. | `O(N)` |
