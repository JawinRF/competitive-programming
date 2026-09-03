# LeetCode 857: Minimum Cost to Hire K Workers

## Idea

Suppose worker `i` sets the pay rate for a group. Every worker is paid at the same rate per unit of quality.

For any worker `j` in the group, the wage requirement is:

```text
(wage[i] / quality[i]) * quality[j] >= wage[j]
```

After dividing by `quality[j]`, this becomes:

```text
wage[i] / quality[i] >= wage[j] / quality[j]
```

Therefore, the worker with the largest `wage / quality` ratio sets the minimum valid rate for the whole group.

Sort all workers by this ratio in increasing order. When the sweep reaches worker `i`, every worker in `[0, i]` can be paid using `p[i].r` because their required ratio is not larger.

For a fixed rate, the cost of a group is:

```text
p[i].r * quality[j1] + p[i].r * quality[j2] + ...
= p[i].r * sum_of_qualities
```

The rate is fixed at this step. We only need to minimize the sum of the qualities of the selected `k` workers.

Maintain a max-heap ordered by quality. The heap keeps the `k` smallest qualities seen so far. When its size becomes `k + 1`, remove its largest quality. The variable `C` stores the sum of all qualities currently in the heap.

If the current worker is removed from the heap, the selected group did not change from the previous step. The current ratio is not smaller than the previous ratio, so this group cannot produce a better answer. The code skips that case.

## Proof

Consider an optimal group of `k` workers. Let worker `i` have the largest `wage / quality` ratio in this group.

1. The smallest rate that satisfies every worker in the group is `p[i].r`.
2. All other workers in the group occur in the sorted prefix `[0, i]`.
3. At index `i`, the heap removes the largest qualities and retains the smallest possible quality sum for the useful candidates in this prefix.
4. If worker `i` remains in the heap, `p[i].r * C` is the minimum cost found for a group whose limiting worker is `i`.
5. If worker `i` is removed, using worker `i` would give a larger quality sum at a rate that is at least as large. It cannot improve the best result already considered.

Every possible optimal group has a worker with the largest ratio. The sweep considers that ratio and the best relevant quality sum. Therefore, the minimum value stored in `ans` is the minimum valid cost.

## Complexity

Sorting takes `O(n log n)` time. Each heap operation takes `O(log k)` time, so the sweep takes `O(n log k)` time.

The total time complexity is `O(n log n)`. The space complexity is `O(n + k)`.
