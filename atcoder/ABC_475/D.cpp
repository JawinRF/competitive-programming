#include <bits/stdc++.h>
using namespace std;

bool samePattern(const string &s, const string &t) {
    vector<int> charToDigit(26, -1);
    vector<int> digitToChar(10, -1);

    for (int i = 0; i < s.size(); i++) {
        int c = s[i] - 'a';
        int d = t[i] - '0';

        if (charToDigit[c] != -1 && charToDigit[c] != d)
            return false;

        if (digitToChar[d] != -1 && digitToChar[d] != c)
            return false;

        charToDigit[c] = d;
        digitToChar[d] = c;
    }

    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    int n = s.size();

    int low = (n == 1 ? 2 : pow(10, n - 1));
    int high = pow(10, n) - 1;

    vector<bool> prime(high + 1, true);

    if (high >= 0) prime[0] = false;
    if (high >= 1) prime[1] = false;

    for (int i = 2; 1LL * i * i <= high; i++) {
        if (!prime[i]) continue;

        for (long long j = 1LL * i * i; j <= high; j += i)
            prime[j] = false;
    }

    for (int x = low; x <= high; x++) {
        if (!prime[x]) continue;

        if (samePattern(s, to_string(x))) {
            cout << x << '\n';
            return 0;
        }
    }

    cout << -1 << '\n';
}