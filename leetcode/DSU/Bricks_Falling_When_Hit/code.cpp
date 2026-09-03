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

class DSU{
    public:
    vector<int> par , sz , stable ;
    DSU(int n){
        par.resize(n) ;
        sz.resize(n) ;
        stable.resize(n) ; 
        for(int i = 0 ; i < n ; ++i){
            par[i] = i ;
            sz[i] = 1 ;
            stable[i] = 0 ;
        }
    }
    int find(int a){
        if(par[a] == a) return a ;
        return par[a] = find(par[a]) ;
    }
    bool unite(int a , int b){
        a = find(a) , b = find(b) ;
        if(a == b) return false ;
        if(stable[a] || stable[b]){
            stable[a] = stable[b] = 1 ;
        }
        if(sz[a] < sz[b]) swap(a,b) ;
        par[b] = a ;
        sz[a] += sz[b] ;
        return true ;
    }
};

class Solution {
public:
    int n , m , q ; 
    int f(int i,int j){
        return i*m + j ; 
    }
    vector<int> hitBricks(vector<vector<int>>& grid, vector<vector<int>>& hits) {
        n = grid.size() , m = grid[0].size() , q = hits.size() ;   
        vector<int> valid(q, 0);
        for(int i = 0; i < q; ++i){
            int x = hits[i][0], y = hits[i][1];

            if(grid[x][y]){
                valid[i] = 1;
                grid[x][y] = 0;
            }
        }
        DSU d(n*m) ;
        for(int j = 0 ;j<m; ++j){
            if(grid[0][j]){
                d.stable[j] = 1 ;
            }
        }
        for(int i = 0 ; i<n ; ++i){
            for(int j = 0  ;j<m ; ++j){
                if(grid[i][j]==1){
                    if(j+1<m && grid[i][j+1])d.unite(f(i,j+1),f(i,j)) ; 
                    if(j-1>=0 && grid[i][j-1])d.unite(f(i,j-1),f(i,j)) ;
                    if(i-1>=0 && grid[i-1][j])d.unite(f(i-1,j),f(i,j)) ;
                    if(i+1<n && grid[i+1][j])d.unite(f(i+1,j),f(i,j)) ;
                }
            }
        }
        vector<int> res ; 
        for(int i = q-1 ; i>=0 ; --i){
            if (!valid[i]){
                res.pb(0) ; 
                continue ;
            }
            int u = hits[i][0] , v = hits[i][1];
            bool change = false ;
            int cnt = 0 ;
            if(!grid[u][v]){
                if(u==0)d.stable[f(u,v)] = 1 ;
                for(int dx:{-1,0,1}){
                    for(int dy:{-1,0,1}){
                        int x = u + dx , y = v + dy ; 
                        if((dx==0 && dy==0) || (dx!=0 && dy!=0))continue ;
                        if(x>=0 && x<n && y>=0 && y<m && grid[x][y] && (d.find(f(x,y))!=d.find(f(u,v)))){
                            int root = d.find(f(x,y));
                            if(!d.stable[root]){
                                cnt += d.sz[root] ; 
                            }else change = true;
                            d.unite(f(x,y),f(u,v)) ;
                        }   
                    }
                }
            }
            grid[u][v] = 1 ;
            res.push_back((change || (u==0))*cnt) ;
        }
        reverse(all(res)) ; 
        return res ;
    }
};