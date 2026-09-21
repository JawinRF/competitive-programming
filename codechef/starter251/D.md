# Make Distinct

## Problem Statement

You are given an array A of length N and an integer K.

In one operation:

- Choose at most K distinct indices.
- Increment the value at each chosen index by 1.

We need to find the minimum number of operations required to make all elements of the array pairwise distinct. That means: $A_i \neq A_j$ for every $i \neq j$.

## My Sort Idea

Consider:
```text
N = 6
K = 3
A = [1, 3, 2, 3, 2, 3]
```

Sort the array:

```text
1 2 2 3 3 3
```

Now for $1 < i < N$ while $a[i] \leq a[i-1]$ do a[i]++. So:

```text
1 2 2 3 3 3 → 1 2 3 4 5 6
```

This is the minimum way to make the sorted array strictly increasing.

## Increment Array d

Now consider how much each index was incremented.

```text
original:  1 2 2 3 3 3
final:     1 2 3 4 5 6
```

So: d = [0, 0, 1, 1, 2, 3]. Ignoring the zeros: [1, 1, 2, 3].

So now the original problem reduces to:

Given this increment array, in one operation choose at most K distinct positive elements and decrement each by 1. Find the minimum number of operations required to make the entire array zero.

## Max-Heap Simulation Idea

Like for simulation my idea is use a priority queue (max heap). Push these: 1 1 2 3. Pick K and decrement each by 1, then insert back if nonzero. Increment count.

For example, with K = 3 we can do:
	
```text
[3, 2, 1, 1]
pick 3, 2, 1 → 2, 1, 0
remaining heap: [2, 1, 1]
```

Then:

```text
pick 2, 1, 1 → 1, 0, 0
remaining heap: [1]
```

Then:

```text
pick 1 → 0
```

So count is: 3.

So problem reduces to do this max heap simulation fast enough. But I do not need to make the heap simulation faster. I can remove it completely.

## Removing the Simulation

Let $R = \text{no of moves required to make } d=[0,0,\ldots,0]$. Then obviously:

$$
R \geq \max\left( \max d[i], \left\lceil\frac{\sum d[i]}{K}\right\rceil \right)
$$

Because one index can only be decremented once in one operation, so $R \geq d[i]$ for every i. Hence $R \geq \max d[i]$. Also, one operation can decrement at most K elements. So in R operations we can do at most $R \cdot K$ total decrements. Hence $R \cdot K \geq \sum d[i]$. So $R \geq \left\lceil\frac{\sum d[i]}{K}\right\rceil$. Therefore $R \geq \max\left( \max d[i], \left\lceil\frac{\sum d[i]}{K}\right\rceil \right)$. But this is only a lower bound. Now we need to show that this R is actually achievable.

## Proof That This R Is Achievable

Then in one heap operation top K elements get decremented by 1. So if our R is sufficiently large then after this operation we must get:

$$
R-1 \geq \max d[i]
$$

and

$$
(R-1)K \geq \sum d[i]
$$

on the updated d array.

The thing to notice is: $R \geq d[i]$ and there can be at most K elements equal to R. Suppose there are K+1 elements equal to R. Then we can say: $\sum d[i] \geq (K+1)R$ because sum d[i] includes K+1 elements equal to R and some other elements. But we know that: $\sum d[i] \leq KR$. But: $\sum d[i] \geq (K+1)R > KR$. So we end up with: $\sum d[i] \leq KR$ and $\sum d[i] > KR$. So contradiction. Hence there can be only at most K elements equal to R.

## What Happens to max d[i]?

Since $R \geq \max d[i]$, if $\max d[i] = R$, then there are at most K elements equal to R. Since our max heap takes the top K elements, all elements equal to R will be chosen. All of them decrease by 1. So after one operation: $\max d[i] \leq R-1$. If $\max d[i] < R$, then already $\max d[i] \leq R-1$. So in both cases: $R-1 \geq \max d[i]$ on the updated array.

## What Happens to the Sum?

If there are at least K positive elements, then one operation decrements exactly K elements. So: $\sum d[i]' = \sum d[i]-K$.

Original equations were: $R \geq \max d[i]$ and $RK \geq \sum d[i]$. Subtract 1 from the first side: $R-1 \geq \max d[i]'$ which is true from the previous argument. Now subtract K from both sides of $RK \geq \sum d[i]$. We get: $RK-K \geq \sum d[i]-K$. So: $(R-1)K \geq \sum d[i]-K$. But: $\sum d[i]' = \sum d[i]-K$. Therefore: $(R-1)K \geq \sum d[i]'$ which also does match.

If fewer than K positive elements remain, say there are p < K positive elements, then we simply decrement all of them. After the operation every positive element is at most R-1. So: $\sum d[i]' \leq p(R-1)$. Since p < K, we get: $\sum d[i]' \leq K(R-1)$. So again: $(R-1)K \geq \sum d[i]'$ holds.

## Repeating the Same Process

So after one operation: R becomes R - 1 and our two properties still remain true: $R-1 \geq \max d[i]'$ and $(R-1)K \geq \sum d[i]'$. So we can repeat this process until: R=0. Then we get: $0 \geq \max d[i]$ and $0 \geq \sum d[i]$. Since all d[i] are non-negative, this means: d[i] = 0 for every i. So all elements become zero.

Hence with any $R \geq \max\left( \max d[i], \left\lceil\frac{\sum d[i]}{K}\right\rceil \right)$ it is possible to finish. To minimize moves use the smallest R. So:

$$
R = \max\left( \max d[i], \left\lceil\frac{\sum d[i]}{K}\right\rceil \right)
$$

is achievable. Therefore it is also the minimum answer.

## Final Algorithm

1. Sort A.

2. Make it strictly increasing greedily.

3. For every index calculate how much it had to be incremented. $d[i] = \text{final}[i] - A[i]$.

4. Calculate: $mx = \max d[i]$ and $sum = \sum d[i]$.

5. Answer: $\max\left( mx, \left\lceil\frac{sum}{K}\right\rceil \right)$ where $\left\lceil\frac{sum}{K}\right\rceil = \frac{sum+K-1}{K}$ using integer division.


## Complexity

Sorting takes: $O(N \log N)$. The remaining scan takes: $O(N)$. So total: $O(N \log N)$ with no max-heap simulation needed.