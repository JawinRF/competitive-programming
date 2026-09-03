#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    struct e {
        double r;
        int q, w, idx;
    };

    struct cmp {
        bool operator()(const e& a, const e& b) const {
            return a.q < b.q; // Keep the worker with the largest quality on top.
        }
    };

    double mincostToHireWorkers(vector<int>& quality, vector<int>& wage, int k) {
        int n = quality.size();
        vector<e> p(n);
        for (int i = 0; i < n; ++i) {
            p[i].q = quality[i];
            p[i].w = wage[i];
            p[i].idx = i;
            p[i].r = ((double)wage[i]) / quality[i];
        }

        sort(p.begin(), p.end(), [&](const e& a, const e& b) {
            return a.r < b.r;
        });

        priority_queue<e, vector<e>, cmp> pq;
        int C = 0;

        double ans = DBL_MAX;
        for (int i = 0; i < n; ++i) {
            C += p[i].q;
            pq.push(p[i]);

            if (pq.size() > k) {
                C -= pq.top().q;
                if (pq.top().idx == p[i].idx) {
                    pq.pop();
                    continue;
                }
                pq.pop();
            }

            if (pq.size() == k) {
                ans = min(ans, p[i].r * C);
            }
        }

        return ans;
    }
};
