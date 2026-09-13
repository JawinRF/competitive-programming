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
