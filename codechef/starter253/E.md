For a fixed value $K$, when can $K$ survive?

$K$ survives exactly when every other value

```math
M\in\{1,2,\ldots,K-1,K+1,\ldots,N\}
```

appears **before $K$ in at least one of the two permutations**.

Why?

Suppose some value $M$ is after $K$ in **both** rows:

~~~text
P: ... K ... M ...
Q: ... K ... M ...
~~~

We can only discard a **leftmost** card.

So, to reach $M$ in either row, we must first discard $K$ from that row.

But discarding a value removes **both copies** of that value.

Therefore, $K$ gets completely removed before we can finish discarding everything else.

Hence $K$ cannot survive.

So the required condition is:

```math
\boxed{ \forall M\neq K,\quad M\text{ is before }K\text{ in }P \quad\text{or}\quad M\text{ is before }K\text{ in }Q }
```

Our job is to use adjacent swaps to achieve this condition **before the discarding starts**.

---

# Aha: We Only Need to Move K Right

An optimal solution can be achieved by moving only $K$, and only **rightward**.

Why?

Suppose some value is originally $d$ positions to the right of $K$.

To make that value appear before $K$, they must cross.

Crossing them requires at least $d$ adjacent swaps if we want $K$ to cross all $d$ intervening elements.

Instead of moving many different elements left, we can simply move $K$ right by $d$ positions.

This costs exactly

```math
d
```

swaps and automatically puts **all intervening values before K**.

So moving $K$ right only helps.

We can move $K$ right in either or both permutations.

---

# Fix a Value K

Suppose the position of $K$ is

```math
x=\mathrm{posP}[K]
```

in $P$, and

```math
y=\mathrm{posQ}[K]
```

in $Q$.

Now suppose we choose the final position of $K$ in $P$ to be $i$, where

```math
i\ge x.
```

Moving $K$ from $x$ to $i$ costs

```math
\boxed{i-x}.
```

---

# What Values Are Still After K in P?

After moving $K$ to position $i$, the values still after it are exactly

~~~text
P[i+1], P[i+2], ..., P[N]
~~~

Every one of these values must appear **before K in Q**.

Otherwise, some value would remain after $K$ in both rows.

---

# Look at Their Positions in Q

For every value $P[j]$ with $j>i$, consider

```math
\mathrm{posQ}[P[j]].
```

Define

```math
\boxed{ S[i]=\max_{j>i}\mathrm{posQ}[P[j]] }
```

with

```math
\boxed{S[N]=0}.
```

So $S[i]$ means:

> The **furthest-right position in Q** among all values that are still after $K$ in $P$.

We can compute this as a suffix maximum:

```math
S[i] = \max\left( S[i+1], \mathrm{posQ}[P[i+1]] \right).
```

---

# What Must Happen in Q?

Currently, $K$ is at position

```math
y=\mathrm{posQ}[K].
```

We need every value still after $K$ in $P$ to become before $K$ in $Q$.

The furthest-right such value is at position

```math
S[i].
```

Therefore, we need $K$ in $Q$ to end up **after position $S[i]$**.

---

## Case 1: $S[i]<y$

Then all required values are already before $K$ in $Q$.

So no movement is needed in $Q$.

Cost in $Q$:

```math
0.
```

Total cost:

```math
\boxed{i-x}.
```

---

## Case 2: $S[i]\ge y$

Then $K$ must move right in $Q$ and cross the required values up to $S[i]$.

The number of swaps needed is

```math
S[i]-y.
```

So the total cost becomes

```math
\boxed{ (i-x)+(S[i]-y) }.
```

Combining both cases:

```math
\boxed{ \text{cost}(i) = (i-x)+\max(0,S[i]-y) }
```

for every

```math
i=x,x+1,\ldots,N.
```

Therefore, for this fixed $K$,

```math
\boxed{ \text{ans}_K = \min_{i=x}^{N} \left[ (i-x)+\max(0,S[i]-y) \right]. }
```

---

# Intuition in One Picture

Initially:

~~~text
P: ... K ... a ... b ... c ...
Q: ... K ... c ... a ... b ...
~~~

Values `a`, `b`, and `c` are after $K$ in both rows, so $K$ cannot survive.

Suppose we move $K$ right in $P$:

~~~text
P: ... a ... b ... K ... c ...
~~~

Now only `c` remains after $K$ in $P$.

Therefore, we only need to ensure that `c` is before $K$ in $Q$.

So move $K$ right in $Q$ until it crosses `c`:

~~~text
Q: ... c ... K ...
~~~

Now every value is before $K$ in at least one row.

Therefore, $K$ can survive.

---

# Core Formula

For fixed $K$:

```math
x=\mathrm{posP}[K], \qquad y=\mathrm{posQ}[K].
```

Build

```math
S[i]=\max_{j>i}\mathrm{posQ}[P[j]].
```

Then try every final position

```math
i\in[x,N].
```

The cost is

```math
\boxed{ (i-x)+\max(0,S[i]-y) }
```

and therefore

```math
\boxed{ \text{ans}_K = \min_{i=x}^{N} \left( i-x+\max(0,S[i]-y) \right). }
```

The whole idea is:

> Move $K$ right in $P$ to reduce the set of values that are after it there.
>
> Whatever values remain after $K$ in $P$ must be crossed by $K$ in $Q$.

for a fixed $K$ we can precompute $S[i]$ in $O(n)$

so for a fixed $K$ we take $n*O(1)$ time

to make it faster

we breakdown the $\max()$ expression

we will get $S[i]-y$ when $s[i]-y>0$ or $s[i]>y$

for increase in 'i' , $S[i]$ can't decrease i.e $S[i]>=S[i+1]$

to minize cost in range $y>S[i]$ where $\max(0,s[i]-y)=0$

so the cost is just = $i-x = i - t$ where $t$ is the first point $> x$ such that $S[i]<y$

other case is $s[i]>y$ so that $\max(0,s[i]-y) = s[i]- y$

so cost becomes = $i - x + s[i] - y$

```math
= ( s[i] + i ) - x - y
```

to minize this we need to minize $s[i]+i$

to do that build a array $C$ such that $C[i] = S[i] + i$

and find min over all $j$ in $[i,t-1]$ of $C[i]$

to find min use sparse table