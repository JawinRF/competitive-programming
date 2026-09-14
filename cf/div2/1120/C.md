# Why Your Construction Works

For a fixed `k`, we need

```math
a_k=\operatorname{mex}\left(\left\{\left\lfloor\frac yk\right\rfloor:y\in B\right\}\right).
```

For the MEX to equal `a_k`, two things must hold:

1. `a_k` must **not occur**.
2. Every `0,1,\dots,a_k-1` must occur.

Now,

```math
\left\lfloor\frac yk\right\rfloor=a_k
```

exactly when

```math
a_k k\le y\le a_k k+k-1.
```

So any valid solution **must exclude** the whole interval

```math
\boxed{[a_k k,\ a_k k+k-1]}.
```

That is exactly what your difference array marks.

You then take

```math
B=\{0,\dots,n-1\}\setminus \bigcup_k [a_k k,a_k k+k-1].
```

So `B` contains **every element that is not forbidden**.

The important part is proving that the smaller values required for the MEX are still present.

We are told that some valid solution `A` exists. Since `A` has mex `a_k`, it cannot contain any number `y` satisfying

```math
\left\lfloor y/k\right\rfloor=a_k.
```

Therefore, for every `k`,

```math
A\cap[a_k k,a_k k+k-1]=\varnothing.
```

Hence every element of `A` survives your deletion process:

```math
\boxed{A\subseteq B}.
```

Since `A` already contains witnesses producing every value

```math
0,1,\dots,a_k-1,
```

and `A\subseteq B`, those witnesses are also in `B`.

At the same time, your construction guarantees that `B` contains **no** number producing `a_k`.

Therefore

```math
\operatorname{mex}\left(\left\{\left\lfloor y/k\right\rfloor:y\in B\right\}\right)=a_k.
```

So your construction is valid for **all `k` simultaneously**.
