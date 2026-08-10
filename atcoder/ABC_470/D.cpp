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
ABC 470 D — permutation P, Q queries: "1 x y" swaps P_x and P_y,
"2" replaces P by its inverse. Print P at the end.
Keep a and b = a^-1 side by side. Query 2 is then just a flag flip,
and query 1 swaps in whichever array the flag names.
*/
// vector<int> f(int n , vector<int> &a){
//     vector<int> res(n+1) ;
//     for(int i = 1 ; i<=n ; ++i){
//         for(int j = 1 ; j<=n ; ++j){
//             if(a[j]==i){
//                 res[i] = j ;
//                 break ;
//             }
//         }
//     }
//     return res ;
// }
vector<int> f(int n , vector<int> &a){
    vector<int> res(n+1) ;
    vector<int> mpp(n+1) ; 
    for(int i = 1 ; i<=n ; ++i){
        mpp[a[i]] = i ;
    }
    for(int i = 1 ; i<=n ; ++i){
        // for(int j = 1 ; j<=n ; ++j){
        //     if(a[j]==i){
        //         res[i] = j ;
        //         break ;
        //     }
        // }
        res[i] = mpp[i] ;
    }
    return res ;
}
void solve(){
    int n ,q  ; cin >> n >> q ;
    vector<int> a(n+1) ; readOff(a,1,n) ;
    // for(int i = 1 ; i<=6 ; ++i){
    //     a = f(n,a) ;
    //     show(a,1) ;
    // }
    vector<int> b = f(n,a) ;

    int curr = 0  ;
    while(q--){
        int t  ; cin >> t ;
        if(t==1){
            int x , y ; cin >> x >> y ;
            if(curr==0){
                b[a[x]] = y ;
                b[a[y]] = x ;
                swap(a[x],a[y]) ;
            }
            else{
                a[b[x]] = y ;
                a[b[y]] = x ;
                swap(b[x],b[y]) ;
            }
            
        }
        else{
            curr ^= 1 ;
        }
    }
    if(curr==0){
        show(a,1) ;
    }
    else{
        show(b,1) ;
    }
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
    //cin >> tc ;

    while(tc--){
        solve();
    }
    auto end = high_resolution_clock::now();
    duration<double> diff = end - start;
    cerr << fixed << setprecision(9) << diff.count() << "\n";
    return 0;
}