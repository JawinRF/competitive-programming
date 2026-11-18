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

const int MAXN = 200005 + 1 ;
vector<int> fact(MAXN) , invFact(MAXN) ;
void precompute(){
    fact[0] = 1 ;
    for(int i = 1 ; i < MAXN ; ++i) fact[i] = fact[i-1] * i % mod ;
    invFact[MAXN-1] = exp(fact[MAXN-1], mod-2, mod) ;
    for(int i = MAXN-2 ; i >= 0 ; --i) invFact[i] = invFact[i+1] * (i+1) % mod ;
}
int nCr(int n, int r){
    if(r < 0 || r > n) return 0 ;
    return fact[n] * invFact[r] % mod * invFact[n-r] % mod ;
}

/*

*/
void solve(){
    int n, m, q ; cin >> n >> m >> q ;
    vector<int> a(n) ; read(a) ;
    if(n == 1){ 
        int x ; 
        for(int i = 0 ; i < q ; ++i){ 
            cout << (m==1 ? 1 : 0) << "\n" ; 
            cin >> x >> x ; 
        }
        return ; 
    }
    vector<int> t(n) ;  
    for(int i = 0 ; i < n ; ++i){
        t[i] = a[i] - a[n-1-i] ;
    }
    map<int,vector<int>> mpp ; 
    vector<int> p(n); p[0] = t[0] ; 
    mpp[p[0]].push_back(0) ;
    for(int i = 1 ; i+1 < n ; ++i) {
        p[i] = p[i-1] + t[i] ;
        mpp[p[i]].push_back(i) ;
    }

    vector<int> zCnt(n) ; 
    for(int i = 0 ; i < n ; ++i){
        if(p[i]==0) zCnt[i] = 1 ;
        if(i>0) zCnt[i] += zCnt[i-1] ;
    }

    
    while(q--){
        int i , v ; cin >> i >> v ;
        i-- ;
        int l = i , r = n-1-i ; 
        int w ;
        if(l<r){
            r-- ; w = v ;
        }
        else if(l>r){
            l-- ; swap(l,r) ; w = -v ;
        }
        else{
            w = 0 ;
        }
        
        // range add w 
        // cnt zeroes
        int t1 = (l>0 ? zCnt[l-1] : 0) ;
        int t2 = zCnt[n-2] - zCnt[r] ; 
        int totalZeroes = t1 + t2 ;
        // for(int j = l ; j <= r ; ++j){
        //     if(p[j]+w==0){
        //         totalZeroes++ ;
        //     }
        // }

        auto &vec = mpp[-w] ;
        auto lft = lower_bound(all(vec), l) ; 
        auto rgt = upper_bound(all(vec), r) ;
        totalZeroes += (rgt - lft) ;
        // compute C( g , m-1)
        // g <= n and n<=2e5
        if(m-1 > totalZeroes){
            put(0) ;
            continue ; 
        }
        int g = totalZeroes ;
        int ans = nCr(g, m-1) ;
        put(ans) ;
    }
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    using namespace chrono;
    auto start = high_resolution_clock::now();
    cerr<<"compiled"<<"\n";

    int tc =  1 ;
    cin >> tc ;
    precompute() ;

    while(tc--){
        solve();
    }
    auto end = high_resolution_clock::now();
    duration<double> diff = end - start;
    cerr << fixed << setprecision(9) << diff.count() << "\n";
    return 0;
}
