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
    void dkj(int src , vector<vector<pairii>> &e , vector<ll> &dist){
        priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<pair<ll,int>>> pq ; 
        dist[src] = 0 ; 
        pq.push({0,src}) ; 
        while(!pq.empty()){
            auto [d,u] = pq.top() ; pq.pop() ; 
            if(d > dist[u]) continue ; 
            for(auto &[v,w] : e[u]){
                if(dist[v] > d + w){
                    dist[v] = d + w ; 
                    pq.push({dist[v],v}) ; 
                }
            }
        }
    }
    long long minimumWeight(int n, vector<vector<int>>& edges, int src1, int src2, int dest) {
        vector<vector<pairii>> e(n) , r(n) ; 
        for(auto &x : edges){
            int u = x[0], v = x[1], w = x[2];
            e[u].eb(v,w) ; 
            r[v].eb(u,w) ;
        }
        vector<ll> sdist1(n,INF), sdist2(n,INF), ddist(n,INF) ;
        dkj(src1, e, sdist1) ;
        dkj(src2, e, sdist2) ;
        dkj(dest, r, ddist) ;
        ll ans = INF ;  
        for(int i = 0 ; i<n ; ++i){
            if(ddist[i]!=INF && sdist1[i]!=INF && sdist2[i]!=INF){
                ans = min(ans,ddist[i]+sdist1[i]+sdist2[i]) ; 
            }
        }
        // show(sdist1,0) ;
        // show(sdist2,0) ;
        // show(ddist,0) ;
        return ans == INF ? -1 : ans ;
    }
};

