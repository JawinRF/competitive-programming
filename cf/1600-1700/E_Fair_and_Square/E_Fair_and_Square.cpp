#include <bits/stdc++.h>
#pragma GCC optimize("O3", "unroll-loops")
#define int long long
#define ll long long
#define eb emplace_back
using namespace std;
int mod = 1e9+7;
const int INF = 1e18;
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
int n ; 
void dfs(int u, int p, vector<vector<int>> &adj, vector<int> &dp){
    dp[u] = 1 ;
    for(auto v : adj[u]){
        if(v==p) continue ; 
        dfs(v,u,adj,dp) ; 
        dp[u] += dp[v] ; 
    }
}
int g = 0 ; 
void dfs2(int u, int p, vector<vector<int>> &adj, vector<int> &a, vector<int> &dp){

    int sqrt = sqrtl(a[u]) ;
    bool is_perfect_square = (sqrt*sqrt==a[u]) ;
    vector<int> comp ; 

    int sum = 0 ;
    for(auto v : adj[u]){
        if(v==p) continue ; 
        dfs2(v,u,adj,a,dp) ; 
        if(is_perfect_square){
            comp.pb(dp[v]) ; 
        }
        sum += dp[v] ;
    }
    int ans1 = 0  , ans2 = 0 ; 
    if(is_perfect_square){
        comp.pb(n-sum-1) ;
        // for(int i = 0 ; i<(int)comp.size() ; ++i){
        //     for(int j = i+1 ; j<(int)comp.size() ; ++j){
        //         ans += comp[i]*comp[j] ; 
        //     }
        // }
        // for(int i = 0 ; i<(int)comp.size() ; ++i){
        //     for(int j = i+1 ; j<(int)comp.size() ; ++j){
        //         for(int k = j+1 ; k<(int)comp.size() ; ++k){
        //             ans += comp[i]*comp[j]*comp[k] ; 
        //         }
        //     }
        // }
        int t1 = 0;  
        int t2 = 0 ;
        for(int i = (int)comp.size()-1 ; i>=0 ; --i){
            ans2 += comp[i]*t2 ;
            t2 += comp[i]*t1 ; 
            ans1 += comp[i]*t1 ; 
            t1 += comp[i] ;
        }

    }
    g += ans1 + ans2; 
}
void solve(){
    cin >> n ; 
    g = 0;
    vector<int> a(n) ; read(a) ;  
    vector<vector<int>> adj(n) ; 
    for(int i = 0 ; i<n-1 ;++i){
        int u , v ; cin >> u >> v ;
        u-- ; v-- ;
        adj[u].pb(v) ;
        adj[v].pb(u) ;
    }
    vector<int> dp(n) ;
    dfs(0,-1,adj,dp) ;
    dfs2(0,-1,adj,a,dp) ;
    put(g) ;
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    // freopen("input.txt","r",stdin);
    // freopen("output.txt","w",stdout);
    using namespace chrono;
    auto start = high_resolution_clock::now();
    cerr<<"compiled"<<"\n";

    int tc =  1 ;
    cin >> tc ;

    while(tc--){
        solve();
    }
    auto end = high_resolution_clock::now();
    duration<double> diff = end - start;
    cerr << fixed << setprecision(9) << diff.count() << "\n";
    return 0;
}