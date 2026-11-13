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
struct Interval{
    int l , r , id ; 
    Interval() = default ;
    Interval(int l , int r , int id) : l(l) , r(r) , id(id) {}
    bool operator<(const Interval &other) const{
        if(l != other.l) return l < other.l ;
        return r < other.r ;
    }
};
void solve(){
    int n , q ; cin >> n >> q ;
    vector<int> cov(n+1) ;
    vector<Interval> Q(q) ; 
    for(int i = 0 ; i<q ; ++i){
        cin >> Q[i].l >> Q[i].r ;
        Q[i].l-- ; Q[i].r-- ;
        cov[Q[i].l]++ ;
        cov[Q[i].r+1]-- ;
        Q[i].id = i ;
    }
    int tot = cov[0]>0;
    for(int i = 1 ; i<=n ; ++i) {
        cov[i] += cov[i-1] ;
        if(cov[i] > 0) tot++ ;
    }
    sort(all(Q)) ; 
    int mxR = -1 , mxRid = -1 ;
    int mxR2 = -1 , mxRid2 = -1 ;
    int j = -1 ; 

    vector<int> singleId(q) ;
    vector<vector<int>> doubleId(q,vector<int>(q)) ;
    for(int i = 0 ; i<n ; ++i){
        while(j+1<q && (Q[j+1].l <= i)){
            if(Q[j+1].r > mxR){
                mxR2 = mxR ;
                mxRid2 = mxRid ;
                mxR = Q[j+1].r ;
                mxRid = Q[j+1].id ;
            }
            else if(Q[j+1].r > mxR2){
                mxR2 = Q[j+1].r ;
                mxRid2 = Q[j+1].id ;
            }
            j++ ; 
        }
        if(cov[i]==1){
            singleId[mxRid]++ ; 
        }
        else if(cov[i]==2){
            doubleId[mxRid][mxRid2]++ ;
            doubleId[mxRid2][mxRid]++ ;
        }
    } 
    int ans = 0 ;
    for(int i = 0 ; i<q ; ++i){
        for(int j = 0 ; j<q ; ++j){
            if(i==j)continue ; 
            ans = max(ans, tot - singleId[i] - singleId[j] - doubleId[i][j]) ;
        }
    }
    put(ans) ; 
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
    // cin >> tc ;

    while(tc--){
        solve();
    }
    auto end = high_resolution_clock::now();
    duration<double> diff = end - start;
    cerr << fixed << setprecision(9) << diff.count() << "\n";
    return 0;
}
