## Idea

There are `m` possible dam vertices.

For every dam except one, place a camera on the edge `(u, p[u])`, where `u` is the dam vertex.

The one dam that does not get a camera must be a dam with minimum depth.

We compute each depth while reading the parents. The problem guarantees `p[i] < i`, so the parent's depth is already known.

Find the minimum-depth dam and skip it. Output every other dam vertex `u`, which represents a camera on `(u, p[u])`.

The answer is therefore `m - 1`.

## Proof

### At least `m - 1` cameras are necessary

If we place `k` cameras, remove those `k` camera edges from the tree.

Removing `k` edges from a tree creates exactly `k + 1` connected components.

Two dams in the same component cannot be distinguished because there is no camera edge between them on their paths from the root.

Therefore, all `m` dams must belong to different components:

`k + 1 >= m`

Thus:

`k >= m - 1`.

### `m - 1` cameras are sufficient

Let `x` be a dam with minimum depth.

Do not place a camera on the edge entering `x`.

For every other dam `u`, place a camera on `(u, p[u])`.

Since `x` has minimum depth, no other dam is an ancestor of `x`.

Every other dam has its own entering edge monitored. Thus, for any two different destination dams, their observed camera sequences are different.

Therefore, `m - 1` cameras are sufficient.

Combining both parts, the minimum number of cameras is exactly:

`m - 1`.

## Complexity

Computing depths and finding the minimum-depth dam takes `O(n + m)` time.

The extra space is `O(n)`.
