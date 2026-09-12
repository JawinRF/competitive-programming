## Idea

Let $S = a + b$. The maximum XOR is always **$S$**.

After $k$ operations, the numbers are $x = a - k$ and $y = b + k$. Their sum stays equal to $S$.

To achieve the maximum XOR with the fewest operations, find the largest submask $x$ of $S$ such that $x \le a$. Build $x$ from the highest bit to the lowest. Include each set bit of $S$ if the result stays at most $a$.

The answer is $S$ and $a - x$.

## Proof

### Maximum XOR

Let $\text{AND}$ denote bitwise AND. For nonnegative integers $x$ and $y$,

$$
x \oplus y = x + y - 2(x \mathbin{\text{AND}} y) \le x + y = S.
$$

Thus, the XOR cannot exceed $S$. Taking $k = a$ gives the pair $(0, S)$, whose XOR is $S$. Therefore, the maximum XOR is exactly $S$.

### Valid values of x

The XOR equals $S$ exactly when

$$
x \mathbin{\text{AND}} (S - x) = 0.
$$

This means that adding $x$ and $S - x$ produces no carries. Every set bit of $x$ must therefore also be set in $S$.

Conversely, if every set bit of $x$ is set in $S$, subtracting $x$ from $S$ requires no borrowing. The remaining set bits form $S - x$, so $x$ and $S - x$ have no set bits in common.

Therefore, the valid values of $x$ are exactly the submasks of $S$ that satisfy $x \le a$.

### Minimum operations

Since $k = a - x$, minimizing the number of operations is equivalent to maximizing $x$.

Process the bits from highest to lowest. If a bit is set in $S$, include it whenever the current value plus that bit is at most $a$.

If including the bit would exceed $a$, no feasible value with the same higher bits can include it. Otherwise, including it is always better than skipping it, because

$$
2^i > \sum_{j=0}^{i-1} 2^j = 2^i - 1.
$$

Thus, no combination of lower bits can compensate for skipping a feasible higher bit. The greedy construction gives the largest valid $x$, so $a - x$ is the minimum number of operations.

## Complexity

The implementation checks bits $30$ through $0$, so it performs $31$ iterations per test case. Time and extra space are both $O(1)$ for this fixed bit range.
