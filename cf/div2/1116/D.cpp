#include <bits/stdc++.h>
#pragma GCC optimize("O3", "unroll-loops")
#define int long long
#define ll long long
#define eb emplace_back
using namespace std;
int mod = 998244353 ;
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
This sequence of run lengths of zeros and ones remains
this is starts are bars
if we have k0 grps of runs of only 0's
then we have k0-1 bars seaprating them
so stars and bars with k0-1 bars and Z1 stars with no group empty
We want to distribute the Z zeroes into k0 non-empty grps
one way is there are Z-1 separators and we need to choose k0-1 of them to place bars

lly for 1s

So formula is (Z-1 choose k0-1) * (O-1 choose k1-1)
*/
const int N = 1e6; 
int fact[N+1] ;  
int invFact[N+1] ;

// to calculate inverse factorial 
// notice that 1/(n-1)! = n/n! 
// lly 1/(n-2)! = (n-1)/(n-1)! in general inv[i] = ( (i+1)*inv[i+1] ) %MOD 
// so we neeed to find inv factorial of N here which is 1e5 then others we can generate iteratively 
// to find 1/N! mod P we can use fermats little theorem that a^(P-1) = 1 mod P where P is prime a P doesn't divide a 
// since N!<P .. P can't divide N! So write 1 as (N!)^(P-1) mod P so we get
// (N!)^(P-2) mod P this can calculated efficiently using modular exponentiation
void solve(){
    int n ; cin >> n ; 
    string s ; cin >> s ;
    int Z = count(all(s),'0') , O = count(all(s),'1') ;
    int k0 = 0 , k1 = 0 ; 
    for(int i = 0 ; i<n ; ){
        int j = i ; 
        while(j<n && s[j] == s[i]) j++ ;
        if(s[i] == '0') k0++ ; 
        else k1++ ;
        i = j ;
    }
    // cout<<Z<<" "<<O<<" "<<k0<<" "<<k1<<"\n" ;

    // ans = (Z-1 choose k0-1) * (O-1 choose k1-1)
    int ans = 1 ; 
    if(k0>0) ans = (ans*fact[Z-1]%mod*invFact[k0-1]%mod*invFact[Z-k0]%mod)%mod ;
    if(k1>0) ans = (ans*fact[O-1]%mod*invFact[k1-1]%mod*invFact[O-k1]%mod)%mod ;
    put(ans) ;

}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    // freopen("input.txt","r",stdin);
    // freopen("output.txt","w",stdout);
    using namespace chrono;
    auto start = high_resolution_clock::now();
    cerr<<"compiled"<<"\n";
    
    // Precompute
    fact[0] = fact[1] = 1 ;
    for(int i = 2 ; i<=N ; ++i) fact[i] = (fact[i-1] * i) % mod ;

    invFact[N] = exp(fact[N],mod-2,mod) ;
    for(int i=N-1 ; i>=0 ; --i)invFact[i] = ((i+1)*invFact[i+1]) % mod ;

    // 
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