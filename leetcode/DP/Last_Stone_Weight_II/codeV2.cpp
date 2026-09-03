#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int lastStoneWeightII(vector<int>& stones) {
        int sum = accumulate(stones.begin(), stones.end(), 0);
        int target = sum / 2;

        vector<bool> dp(target + 1, false);
        dp[0] = true;

        for (int x : stones) {
            for (int j = target; j >= x; j--) {
                dp[j] = dp[j] || dp[j - x];
            }
        }

        for (int j = target; j >= 0; j--) {
            if (dp[j])
                return sum - 2 * j;
        }

        return 0;
    }
};
