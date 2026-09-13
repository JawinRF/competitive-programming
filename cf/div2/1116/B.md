# B. Domino Tiles

[Problem link](https://codeforces.com/contest/2256/problem/B)

## Idea

Let the completed row be `a[0], a[1], ..., a[n - 1]`. The dominoes at positions `i` and `i + 1` have weights

```math
a[i] + a[i + 1]
\quad\text{and}\quad
a[i + 1] + a[i + 2].
```

They must be different. After cancelling the common value `a[i + 1]`, this condition becomes

```math
a[i] \ne a[i + 2].
```

Every tile is either `0` or `1`, so the third tile must be the opposite of the first tile:

```math
a[i + 2] = 1 - a[i].
```

Therefore, the first two tiles determine the whole row. There are only four possible infinite patterns:

```text
00 -> 001100110011...
01 -> 011001100110...
10 -> 100110011001...
11 -> 110011001100...
```

For each pattern, check whether every known character in `s` agrees with it. A question mark agrees with any value. The number of matching patterns is the answer.

## Proof

Consider three consecutive tiles `a[i], a[i + 1], a[i + 2]`. The two neighboring domino weights are different exactly when

```math
a[i] + a[i + 1] \ne a[i + 1] + a[i + 2].
```

Removing the common middle tile gives `a[i] != a[i + 2]`. Since both values are binary, this is equivalent to `a[i + 2] = 1 - a[i]`.

Starting from any choice of the first two tiles, this rule fixes every later tile. The four choices for the first two tiles produce exactly the four patterns listed above. Each valid completion must be one of these patterns, and each pattern that agrees with all non-question-mark characters is a valid completion.

Thus, counting the matching patterns gives exactly the number of valid replacements.

## Complexity

There are four patterns, and each is checked in `O(n)` time. The total complexity is `O(n)` with `O(1)` extra space per test case.

## Implementation

```cpp
#include <bits/stdc++.h>
using namespace std;

void solve() {
    int n;
    string s;
    cin >> n >> s;

    const string patterns[] = {
        "0011", "0110", "1001", "1100"
    };

    int answer = 0;

    for (const string& pattern : patterns) {
        bool valid = true;

        for (int i = 0; i < n; ++i) {
            if (s[i] != '?' && s[i] != pattern[i % 4]) {
                valid = false;
                break;
            }
        }

        answer += valid;
    }

    cout << answer << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int test_cases;
    cin >> test_cases;

    while (test_cases--) {
        solve();
    }
}
```
