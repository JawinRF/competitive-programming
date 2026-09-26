#include <bits/stdc++.h>
#pragma GCC optimize("O3", "unroll-loops")
#define ll long long
#define eb emplace_back
using namespace std;
ll mod = 1e9+7;
ll int INF = 1e18;
#define pb push_back
#define ull unsigned long long
#define all(s) (s).begin(), (s).end()
#define pairii pair<int,int>
#define getsum(v) accumulate(v.begin(), v.end(), 0LL)
#define read(v) for (auto &ele : v) cin >> ele;
#define readOff(v, offset, endoff) for(int traveller = offset ; traveller <= endoff ; ++traveller) cin >> v[traveller];
#define show(v, begin) for(int showj = begin ; showj < (ll)v.size() ; ++showj) cout << v[showj] << ((showj == (ll)v.size() - 1) ? "\n" : " ");
#define read2d(v) for (auto &row : v) for (auto &x : row) cin >> x;
#define show2d(v) for (const auto &row : v) { for (const auto &x : row) cout << x << " "; cout << "\n"; }
#define sputdouble(val) cout << fixed << setprecision(16) << val <<" "
void yes(bool a) { cout << (a ? "yes" : "no") << "\n"; }
void YES(bool a) { cout << (a ? "YES" : "NO") << "\n"; }
void Yes(bool a) { cout << (a ? "Yes" : "No") << "\n"; }
#define put(billu) cout << billu << "\n";
#define sput(billu) cout << billu << " ";
#define rep(i, InclusiveEnd) for(int i = 0 ; i <= InclusiveEnd; ++i)
struct Edge {int u, v, w;};
int dx[] = {-1,1,0,0} , dy[] = {0,0,-1,1} ;
int ceil_div(int a, int b){ assert(b > 0); return (a + b - 1) / b; }
int floor_div(int a, int b){ assert(b > 0); return a / b; }
int MSB(int p){ assert(p > 0);return 63 - __builtin_clzll(p) ;}
pair<int,int> intersection(pair<int,int> &p1,pair<int,int> &p2){return {max(p1.first,p2.first),min(p1.second,p2.second)}; } 
void unique(vector<int> &b) {sort(b.begin(), b.end());auto it = std::unique(b.begin(), b.end());b.erase(it, b.end());}
ll exp(ll x, ll n, ll m) {assert(n >= 0);x %= m;ll res = 1;while (n > 0) {if (n % 2 == 1) {res = res * x % m;}x = x * x % m;n /= 2;}return res;}
int addmod(int a, int b) {assert((a + b) < 2 * mod);a += b;if (a >= mod) a -= mod;return a;}
template <typename T, typename U>
ostream& operator<<(ostream& os, const pair<T, U>& p) {
    os << p.first << " " << p.second << "\n" ;
    return os;
}

/*

*/
class Solution {
public:
    int dijkstra(int n, vector<vector<pair<int,int>>>& graph, int source, int destination) {
        vector<int> dist(n, INT_MAX);
        dist[source] = 0;
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
        pq.push({0, source});
        
        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();
            if (d > dist[u]) continue;
            for (auto [v, w] : graph[u]) {
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                }
            }
        }
        return dist[destination];
    }
    vector<vector<int>> modifiedGraphEdges(int n, vector<vector<int>>& edges, int source, int destination, int target) {
        vector<vector<pair<int,int>>> graph(n);
        vector<vector<int>> BadEdges, GoodEdges;

        int MAX = 2'000'000'000;
        for(auto &edge : edges) {
            int u = edge[0], v = edge[1], w = edge[2];
            if(w == -1) {
                BadEdges.push_back({u, v, -1});
                continue;
            }
            GoodEdges.push_back({u, v, w});
            graph[u].push_back({v, w});
            graph[v].push_back({u, w});
        }
        int d0 = dijkstra(n, graph, source, destination);
        if(d0 < target) {
            return {} ; 
        }
        else if(d0==target){
            for(auto &edge : BadEdges) {
                edge[2] = MAX ;
                GoodEdges.push_back(edge);
            }
            return GoodEdges ;
        }
        else{
            int currD = d0 ;
            bool done = false ;
            for(auto &edge : BadEdges) {
                if(done) {
                    edge[2] = MAX ; 
                    GoodEdges.push_back(edge);
                    continue ;
                }
                edge[2] = 1;
                graph[edge[0]].push_back({edge[1], 1});
                graph[edge[1]].push_back({edge[0], 1});
                int res = dijkstra(n, graph, source, destination);
                currD = res ;
                if(currD<=target){
                    done = true ;
                    edge[2] += (target - currD) ;
                }
                GoodEdges.push_back(edge);
            }
            if(done)return GoodEdges ; 
        }
        return {} ;
    }
};