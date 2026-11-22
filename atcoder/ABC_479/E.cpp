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
struct custom_hash {
    static uint64_t fixed_random;

    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }
    template <class T>
    size_t operator()(T x) const {
        return splitmix64(x + fixed_random);
    }

    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);

        uint64_t mixed1 = splitmix64(h1 + fixed_random);
        uint64_t mixed2 = splitmix64(h2 + fixed_random);

        return mixed1 ^ (mixed2 >> 1);
    }
};

uint64_t custom_hash::fixed_random = std::chrono::steady_clock::now().time_since_epoch().count();
/*
If subtree is larger component then we 
we mark that node as 1
if not then zero and add xv to global sum

second dfs we collect sv and add them to each node in subtree 
if marked as 1 
else we add -xv to each node in subtree
*/
int global_sum = 0;
int n ; 
int dfs(int u , int p ,vector<int> adj[] , unordered_map<pair<int,int>, int , custom_hash> &sv, vector<int> &m){
    int subtree_size = 1;
    for(int v : adj[u]){
        if(v == p) continue;
        subtree_size += dfs(v, u, adj, sv, m);
    }
    int other = n - subtree_size;
    if(subtree_size > other){
        m[u] = 1;
    }
    else{
        m[u] = 0;
        global_sum += sv[make_pair(min(u,p), max(u,p))];
    }
    return subtree_size;
}
void dfs2(int u , int p ,vector<int> adj[] , vector<int> &m,
    int downstream_sum, vector<int> &x, unordered_map<pair<int,int>, int , custom_hash> &mp){
    
    int sv = mp[make_pair(min(u,p), max(u,p))];
    if(m[u])downstream_sum += sv ;
    else downstream_sum -= sv ;

    x[u] += downstream_sum;

    for(int v:adj[u]){
        if(v==p) continue;
        dfs2(v, u, adj, m, downstream_sum, x, mp);
    }
}
void solve(){
    cin >> n; 
    vector<int> adj[n];
    unordered_map<pair<int,int>, int , custom_hash> mp;
    for(int i = 0; i<n-1 ; ++i){
        int u, v ;cin >> u >> v; --u; --v;
        adj[u].pb(v);
        adj[v].pb(u);
        mp[make_pair(min(u,v), max(u,v))] = i+1;
    }
    
    vector<int> x(n) , m(n); 

    dfs(0, -1, adj, mp, m);
    dfs2(0, -1, adj, m, 0, x, mp);
    for(int i = 0 ; i<n ; ++i){
        x[i] += global_sum;
    }
    show(x, 0);
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
    // cin >> tc ;

    while(tc--){
        solve();
    }
    auto end = high_resolution_clock::now();
    duration<double> diff = end - start;
    cerr << fixed << setprecision(9) << diff.count() << "\n";
    return 0;
}