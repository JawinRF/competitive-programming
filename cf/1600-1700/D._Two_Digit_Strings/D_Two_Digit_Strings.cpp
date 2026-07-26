#include <bits/stdc++.h>
#pragma GCC optimize("O3", "unroll-loops")
#define int long long
#define ll long long
#define eb emplace_back
using namespace std;
int mod = 1e9+7;
const int INF = 1e17;
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
    // dp stores the the maximum possible length of the resulting equal strings a[...i] and b[...j]
    // dp2 stores the maximum possible length of the resulting equal strings a[...i] and b[...j] with the last digit being k
 
    // pref[i] - pref[i'] == pref[j] - pref[j'] modulu 10 
    // pref[i] - pref[j] == pref[i'] - pref[j'] modulu 10
 
    // dp[i][j] = max 1 + dp[i'][j'] if pref[i] - pref[j] == pref[i'] - pref[j'] modulu 10 over all 
    // dp[i][0] and dp[0][j] are 0 
*/
void solve(){
    string a, b ; cin >> a >> b ;
    int n = a.length() , m = b.length() ;

    vector<vector<int>> prev(10, vector<int>(m+1, -INF)), curr(10, vector<int>(m+1, -INF));
    for(int j = 0 ; j <= m ; ++j) prev[0][j] = 0;

    vector<int> pref(n+1,0) , pref2(m+1,0) ;
    for(int i = 1 ; i<=n ; ++i) pref[i] = (pref[i-1] + (a[i-1]-'0')) ;
    for(int j = 1 ; j<=m ; ++j) pref2[j] = (pref2[j-1] + (b[j-1]-'0')) ;

    int ans = -1 ;
    for(int i = 1 ; i<=n ; ++i){
        curr[0][0] = 0 ;
        for(int k = 1 ; k < 10 ; ++k) curr[k][0] = -INF ;
        for(int j = 1 ; j<=m ; ++j){
            int t = ((pref[i]-pref2[j])%10+10)%10;
            int prv = prev[t][j-1] ;
            if(prv != -INF) ans = 1 + prv ;
            else ans = -INF ; 
            for(int k = 0 ; k<10 ; ++k){
                curr[k][j] = max(prev[k][j], curr[k][j-1]) ;
            }
            curr[t][j] = max(curr[t][j], ans) ;
        }
        swap(curr, prev) ;
    }
    put((ans<1?-1:ans));
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