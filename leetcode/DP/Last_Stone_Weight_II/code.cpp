#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    vector<int> s[2];

    void f(vector<int> &stones, int i, int c, int setType, int m) {
        if (i == m) {
            s[setType].push_back(c);
            return;
        }

        f(stones, i + 1, c - stones[i], setType, m);
        f(stones, i + 1, c + stones[i], setType, m);
    }

    int lastStoneWeightII(vector<int>& stones) {
        s[0].clear();
        s[1].clear();

        int sz = stones.size() / 2;
        f(stones, 0, 0, 0, sz);
        f(stones, sz, 0, 1, (int)stones.size());

        sort(s[0].begin(), s[0].end());
        s[0].erase(unique(s[0].begin(), s[0].end()), s[0].end());

        sort(s[1].begin(), s[1].end());
        s[1].erase(unique(s[1].begin(), s[1].end()), s[1].end());

        // -s1+s2
        // -s1-s2
        // +s1+s2 = -(-s1-s2)
        // +s1-s2 = -(-s1+s2)

        int mn = 10000;
        for (auto &x : s[0]) {
            for (auto &y : s[1]) {
                int t1 = -x + y;
                int t2 = -x - y;
                int t3 = -t1;
                int t4 = -t2;
                if (t1 >= 0) mn = min(mn, t1);
                if (t2 >= 0) mn = min(mn, t2);
                if (t3 >= 0) mn = min(mn, t3);
                if (t4 >= 0) mn = min(mn, t4);
            }
        }
        return mn;
    }
};
