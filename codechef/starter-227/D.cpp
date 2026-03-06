#include <iostream>
#include <vector>

using namespace std;

const int MOD = 998244353;

long long pow2[1000005];

void precompute_pow() {
    pow2[0] = 1;
    for (int i = 1; i < 1000005; i++) {
        pow2[i] = (pow2[i - 1] * 2) % MOD;
    }
}

void solve() {
    int N;
    if (!(cin >> N)) return;
    vector<long long> A(N);
    for (int i = 0; i < N; i++) cin >> A[i];

    // dp[i] = number of subsequences where A[i] is the LAST 
    // element chosen by the greedy "good" algorithm.
    vector<long long> dp(N, 0);
    long long ans = 0;

    for (int i = 0; i < N; i++) {
        // Option 1: A[i] is the very first element of the greedy sequence.
        // Elements before index i are NOT in the subsequence C.
        dp[i] = 1;

        // Option 2: A[i] follows a previous greedy element A[j].
        for (int j = 0; j < i; j++) {
            if (A[i] > 2 * A[j]) {
                // We need to count how many elements between j and i 
                // satisfy A[k] <= 2 * A[j]. These are the only ones
                // that can optionally be in C without blocking A[i].
                int skippable = 0;
                bool blocked = false;
                for (int k = j + 1; k < i; k++) {
                    if (A[k] <= 2 * A[j]) {
                        skippable++;
                    } else {
                        // There is an element A[k] > 2*A[j] before A[i].
                        // For A[i] to be the NEXT greedy pick, A[k] 
                        // MUST be absent from the subsequence C.
                        // So we don't increment skippable, and it doesn't 
                        // contribute a factor of 2.
                    }
                }
                
                long long ways = (dp[j] * pow2[skippable]) % MOD;
                dp[i] = (dp[i] + ways) % MOD;
            }
        }

        // Each subsequence ending greedily at A[i] can be finished 
        // by any subset of the remaining (N - 1 - i) elements.
        long long ways_to_end = (dp[i] * pow2[N - 1 - i]) % MOD;
        ans = (ans + ways_to_end) % MOD;
    }

    cout << ans << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    precompute_pow();
    int T;
    cin >> T;
    while (T--) {
        solve();
    }
    return 0;
}
