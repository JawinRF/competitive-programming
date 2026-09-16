#include <bits/stdc++.h>
#pragma GCC optimize("O3", "unroll-loops")
#define int long long
#define ll long long
#define eb emplace_back
using namespace std;
int mod = 998244353;
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
void solve(){
    int n ; cin >> n ;  
    int m =  MSB(n) + 1 ;
    vector<int> s(m);   
    for(int i = 0 ; i<m ; ++i){
        if(i<MSB(n)){
            s[i] = 1ll<<(i) ; 
        }
        else{
            s[i] = n - (1ll<<(i)) + 1 ;
        }
    }
    // fix the answer i.e the size of the largest grp
    // we have N possible sizes of groups
    // count(K) = F(K) - F(K-1)
    // we F(k) =  number of ways to make grps of size at most k 

    // nCr = nC(r-1)

    //  = n!/((n-r)! * r! )
    //  = n!/((n-r)!*(r-1)! * r)
    //   = (nC(r-1)) * ((n-r+1)/r)
    vector<vector<int>> prefixNCr(m, vector<int>(n+1, 0)) ;
    for(int i = 0 ; i<m ; ++i){
        prefixNCr[i][0] = 1 ;   
        int ncR = 1 ; // nC0 = 1
        for(int r = 1 ; r<=n ; ++r){
            if(r>s[i]){
                prefixNCr[i][r] = prefixNCr[i][r-1] ;
                continue ;
            }
            int newTerm ; // ncr = nc(r-1) * (n-r+1)/r
            newTerm = ncR * (s[i]-r+1) % mod ;
            newTerm = newTerm * exp(r, mod-2, mod) % mod ; // r^-1 = r^(mod-2) mod mod
            ncR = newTerm ;
            prefixNCr[i][r] = (prefixNCr[i][r-1] + newTerm) ;  
            if(prefixNCr[i][r]>=mod) prefixNCr[i][r] -= mod ;
        }
    }
    auto F = [&](int k){
        int ans = 1 ; 
        for(int i = 0 ; i<m ; ++i){
            int grpSize = s[i] ;  
            // ans *= grpSizeC(k) + grpSizeC(k-1) + grpSizeC(k-2) + ... + grpSizeC(0)
            ans *= prefixNCr[i][k] ;
            ans %= mod ;
        }
        return ans ;
    };
    int ans = 0 ; 
    for(int grpSize = 1 ; grpSize <= n ; ++grpSize){
        int count = F(grpSize) - F(grpSize-1) ;
        if(count < 0) count += mod ;
        ans = (ans + count*grpSize) % mod ;
    }
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