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
Maximize array size
the maximum element can never be removed
which means that it forms a hard boundary such that elements to the left of maximum element cannot interact with elements to the right of maximum element
so we can split the array into two parts and solve them independently
we can remove the element element immedeiate to left or 
*/
// int func(vector<int> &v, int l, int r){
//     if(r-l<=1) return 0 ;
//     int idx = max_element(v.begin() + l , v.begin() + r + 1) - v.begin() ;
    
//     int res = min((idx-l) + func(v,idx+1,r), (r-idx) + func(v,l,idx-1)) ;
//     return res  ;
// }
int pos[500005] ;
int sparse[20][500005] ;
int query(int l, int r){
    int len = r-l+1 ;
    int p = MSB(len) ;
    return max(sparse[p][l], sparse[p][r-(1<<p)+1]) ;
}
void build(vector<int> &v){
    int n = v.size() ;
    for(int i = 0 ; i < n ; ++i) sparse[0][i] = v[i] ;
    for(int p = 1 ; p < 20 ; ++p){
        for(int i = 0 ; i + (1<<p) - 1 < n ; ++i){
            sparse[p][i] = max(sparse[p-1][i], sparse[p-1][i+(1<<(p-1))]) ;
        }
    }
}
int func(vector<int> &v, int l, int r){
    if(r-l<=1) return 0 ;
    // int idx = max_element(v.begin() + l , v.begin() + r + 1) - v.begin() ;
    int mx = query(l,r) ;
    int idx = pos[mx] ;// given input is a perm
    int res = min((idx-l) + func(v,idx+1,r), (r-idx) + func(v,l,idx-1)) ;
    return res  ;
}
void solve(){
    int n ; cin >> n ;  
    vector<int> v(n) ; read(v) ;
    build(v) ;
    for(int i = 0 ; i < n ; ++i) pos[v[i]] = i ;
    put(func(v,0,n-1)) ;
}
/*
    T(n) =  T(idx-l) + T(r-idx) + 2*O(1)
    T(2) = O(1)  , T(1) = O(1) 
    T(0) = O(1)
    T(n) = 2*T(n/2) + O(1)
    if we take skewed partition then T(n) = T(n-1) + O(1) = O(n) 
    T(n) = a*T(n/b) + n^c where c = log a base b
    a=2 , b = 2 , c = 0 b^0 = 1 2>a 
    hence third case n^1 = O(n)
*/
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