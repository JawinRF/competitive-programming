#include <bits/stdc++.h>
using namespace std;

class Solution {
public:
    int shortestPathLength(vector<vector<int>>& adj) {
        int n = adj.size();
        // lets suppose one
        // 00000 , i = -
        // 0...i...0 , i = 0
        // state valid ending with v
        // then extend it using v's adajcnecy list
        // given
        // lets say u is adjacent then u reach a state
        // (curr | (1<<u),u)
        unsigned int INF = ~(unsigned)0;
        vector<vector<int>> dist(1<<n, vector<int>(n, -1));
        queue<pair<int,int>> q;
        for(int i = 0; i < n; i++) {
            dist[1<<i][i] = 0;
            q.emplace(1<<i, i);
        }
        int finalState = (1<<n)-1;
        while(!q.empty()) {
            auto [mask,u] = q.front(); q.pop();
            int d = dist[mask][u];
            if(mask == finalState)
                return d;        // first time we see full mask, it's optimal
            for(int v : adj[u]) {
                int m2 = mask | (1<<v);
                if(dist[m2][v] < 0) {
                    dist[m2][v] = d + 1;
                    q.emplace(m2, v);
                }
            }
        }
        return 0;
    }
};