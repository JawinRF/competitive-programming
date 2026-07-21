#include <bits/stdc++.h>
#pragma GCC optimize("O3", "unroll-loops")
#define int long long
#define ll long long
#define eb emplace_back
using namespace std;
int mod = 1e9+7;
const int INF = 1e15 ;
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
since the leftmost boundary cant have a fixed value we can fix it to anything
boundary is k
let p[k-1] = -inf
and p[k] = derived/given
a[k] = p[k] -p[k-1] = given/derived + inf
now this is important cz this means that we can have -inf as prefsum before leftboundary
so we can do -inf + a[j] + a[j+1] +... for those given values and come back to -inf 
*/
const int N = 2e5 + 1 ; 
int ans[N]; 

void solve(){
    int n ; cin >> n ;
    vector<bool> s(n) ,pref_known(n) ;
    vector<int> a(n) , c(n) ,p(n) ; 
    string t ; cin >> t ;
    for(int i = 0 ; i < n ; ++i) s[i] = (t[i] == '1') ;
    read(a) ; read(c) ;
    if(s[0] && a[0] != c[0]) {
        YES(0) ;  
        return ; 
    }
    pref_known[0] = 1 ; a[0] = c[0] ; s[0] = 1 ;
    p[0] = c[0] ;
    //mark known values of pref_sums
    for(int i = 1 ; i < n ; ++i){
        if(c[i] != c[i-1]) {
            p[i] = c[i] ; 
            pref_known[i] = true ; 
        }
    }
    // p[i+1] = p[i] + a[i] 
    for(int i = 0 ; i+1<n ; ++i){
        if(pref_known[i] && s[i+1]){
            p[i+1] = p[i] + a[i+1] ; 
            pref_known[i+1] = true ;
        }
    }
    // p[i-1] = p[i] - a[i] ; 
    for(int i = n-1 ; i>0 ; --i){
        if(pref_known[i] && s[i]){
            p[i-1] = p[i] - a[i] ;
            pref_known[i-1] = true ;
        }
    }
    for(int i = 1 ; i<n ; ++i){
        if(!pref_known[i]){
            if(s[i]) {
                p[i] = p[i-1] + a[i] ;
            }
            else {
               p[i] = -INF ; 
            }
        }
    }
    bool good = true ; 
    int curr_p = a[0] , curr_mx = a[0] ; 
    
    for(int i = 1 ; i<n ;++i){
        int tmp = p[i] - p[i-1] ; 
        curr_p += tmp ;
        curr_mx = max(curr_mx, curr_p) ;
        
        good &= curr_mx == c[i] ;
        
        if(s[i])good &= (a[i]==tmp) ; 
        a[i] = tmp ; 
    }
    
    YES(good) ; 
    if(good)show(a,0) ;
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