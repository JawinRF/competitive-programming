## Idea

Build the prefix-sum array instead of choosing the elements of `a` directly.

Use zero-based indices, as in the implementation. Let $p_{-1}=0$. Then

$$
a_i=p_i-p_{i-1}.
$$

Each character determines the sign of $p_i$. Since every $a_i$ must be non-zero, consecutive prefix sums must be different. The cost is

$$
\max_{0 \le i \le n-1} \left|p_i-p_{i-1}\right|.
$$

At each position, try all possible next prefix sums and keep only those whose signs match the next character. For each resulting state, keep the minimum cost so far.

If two paths reach the same position and prefix sum, their future choices are identical. Therefore, keep the path with the smaller maximum jump so far. Choosing the smallest absolute prefix sum is not sufficient.

## Why prefix sums from -3 to 3 are enough

Start with any valid prefix-sum array, including the fixed initial value $p_{-1}=0$. Suppose its maximum value is $M$, where $M>3$. Choose an occurrence of $M$.

Its existing neighbors are strictly smaller than $M$, because consecutive prefix sums cannot be equal. There are three cases:

1. **Neither neighbor equals $M-1$.** Replace $M$ with $M-1$. Every neighbor is at most $M-2$, so the affected differences decrease and remain non-zero.

2. **A neighbor equals $M-1$, but no neighbor equals $M-2$.** Replace $M$ with $M-2$. The difference from a neighbor equal to $M-1$ remains $1$. Every other neighbor is at most $M-3$, so its difference decreases. No affected difference becomes zero.

3. **One neighbor equals $M-1$ and the other equals $M-2$.** Replace $M$ with $M-3$. The two differences change from $1,2$ to $2,1$. Their maximum stays equal to $2$, so the array's cost does not increase.

At an endpoint, consider only existing neighbors. The first two cases cover an element with only one neighbor.

All replacement values are positive because $M>3$. Thus, the signs remain correct, consecutive prefix sums remain different, and the cost does not increase. The fixed initial zero is never changed.

Repeat until every prefix sum is at most $3$. Each replacement strictly decreases the sum of the absolute prefix sums, so the process terminates. Apply the same argument with signs reversed to raise any prefix sum below $-3$.

Therefore, every valid solution can be converted into one whose prefix sums lie in

$$
\{-3,-2,-1,0,1,2,3\}
$$

without increasing its cost. In particular, an optimal solution exists within these seven states whenever a solution exists.

The smaller range $[-2,2]$ is not sufficient. For `+----+`, the prefix sums

```text
p = [1, -1, -2, -3, -1, 1]
a = [1, -2, -1, -1,  2, 2]
```

give cost $2$. With cost at most $2$, the first sign change must be $1\to-1$. If negative prefix sums are restricted to $-1,-2$, the four negative positions must alternate as $-1,-2,-1,-2$. The final jump to a positive value then costs at least $3$.

## DP state

Define $dp[i][f(x)]$ as the minimum cost for the first $i+1$ characters when $p_i=x$, where

$$
f(x)=x+3.
$$

This maps prefix sums from $-3$ through $3$ to indices from $0$ through $6$. An unreachable state has value `INF`.

### Initialization

The first element is $a_0=p_0$, so its cost is $|p_0|$.

- If `s[0]` is `+`, initialize the states for $1,2,3$ with costs $1,2,3$.
- If `s[0]` is `-`, initialize the states for $-1,-2,-3$ with costs $1,2,3$.
- If `s[0]` is `0`, leave every state unreachable, because this would require $a_0=0$.

### Transition

From each reachable state with current prefix sum $x$, try every next prefix sum $nx$ in $[-3,3]$.

Reject $nx=x$, because it would create a zero array element. Also reject any value whose sign does not match `s[i+1]`.

The new array element is $nx-x$, so update

$$
dp[i+1][f(nx)] = \min\left(dp[i+1][f(nx)],\ \max\left(dp[i][f(x)],|nx-x|\right)\right).
$$

### Answer

Take the minimum value in the last DP row. If every state is unreachable, output $-1$.

For example, `+--+` permits $p=[1,-1,-2,1]$, which gives $a=[1,-2,-1,3]$ and cost $3$.

## Correctness

The initialization covers every valid first prefix sum in the bounded range with its exact cost.

Assume a DP row stores the minimum cost for every reachable state. Every valid next prefix sum is considered by the transition. The sign check enforces the required character, and the inequality check ensures that the new array element is non-zero. Taking the maximum includes the new jump in the cost. Taking the minimum selects the best path to the next state.

A more expensive path to the same state can be discarded: any continuation is available to both paths, and starting with a smaller cost cannot produce a larger final cost under the maximum operation.

By induction, the last row gives the minimum cost for every possible final prefix sum in the bounded range. The bound proved above preserves an optimal solution, so the minimum over that row is the answer to the original problem. If no state is reachable, no valid array exists.

## Complexity

Each position checks at most $7\times7=49$ transitions. Time is $O(49n)=O(n)$, and the implementation stores $7n$ DP entries, using $O(n)$ space.
