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
I can't enumerate the ~N²/2 edges, but I can enumerate the possible weights
i.e every edge weight is at most 10^6

m = 1e6 then
for each weight w . iterations m/w
which is O(m log m) complexity

*/
class DSU{
    int n ; 
    vector<int> par,sz ;
    
    public:
        DSU(int n){
            this->n = n ;
            par = vector<int> (n) ;  
            sz = vector<int> (n,1) ;
            iota(par.begin(),par.end(),0) ; 
        }
        int find(int v){
            if(v==par[v]){
                return v ;
            }
            int res = find(par[v]) ;
            par[v] = res ;
            return par[v] ; 
        }
        bool unite(int u , int v){
            int a = find(u) ;  
            int b = find(v) ;
            
            if(a==b)return false ; 
        
            // force a to have the smaller size
            if(sz[a]>sz[b]){
                swap(a,b) ; 
            }
            sz[b] += sz[a] ;
            par[a] = b ; 
            
            return true;
        }
        
};
int m = 1'000'000 ;
void solve(){
    int n ;  cin >> n ;
    vector<int> a(n) ; read(a) ;
    vector<bool> mark(m+1,0) ;
    for(int x:a){
        mark[x] = 1 ;
    }
    DSU d(n);
    sort(all(a)) ; 
    auto getIdx = [&](int x){
        return lower_bound(all(a),x) - a.begin() ;
    };
    int wSum = 0 ;
    for(int k = m  ; k>=1 ; --k){
        vector<int> collections ; 
        for(int j = k ; j<=m ; j+=k){
            if(mark[j]){
                collections.pb(j) ;
            }
        }
        for(int i = 1 ; i<(int)collections.size() ; ++i){
            if(d.unite(getIdx(collections[i]),getIdx(collections[0]))){
                wSum += k ;
                // mark[collections[i]] = 0 ; 
            }
        }
    }
    put(wSum) ;
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