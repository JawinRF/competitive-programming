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

int inv = 0 ;
vector<int> mergeS(int l , int r , vector<int> &p){
    if (l==r){
        return {p[l]} ;
    }
    int m = (l+r)/2 ;
    vector<int> t1 = mergeS(l, m, p) ;
    vector<int> t2 = mergeS(m+1, r, p) ;

    vector<int> res ;
    int i = 0 , j = 0 ;  
    while(i < t1.size() && j < t2.size()){
        if(t1[i] <= t2[j]){
            res.pb(t1[i]) ;
            i++ ;
        }else{
            res.pb(t2[j]) ;
            j++ ;
            inv += (t1.size() - i) ;
        }
    }
    while(i < t1.size()){
        res.pb(t1[i]) ;
        i++ ;
    }
    while(j < t2.size()){
        res.pb(t2[j]) ;
        j++ ;
    }
    return res ;
}
void solve(){
    int n ; cin >> n ;
    vector<int> s(n), t(n) ; read(s) ; read(t) ;
    
    int ans = 0 ;

    vector<int> a(n),b(n) ;
    iota(all(a), 0) ;
    iota(all(b), 0) ;
    sort(all(a), [&](int i, int j){
        return s[i] < s[j];
    }) ;
    sort(all(b), [&](int i, int j){
        return t[i] < t[j];
    }) ;
    vector<int> rs(n) , rt(n) ; 
    for(int i = 0 ; i < n ; ++i){
        rs[a[i]] = i ;
        rt[b[i]] = i ;
    }
    vector<int> p(n) ;
    for(int i = 0 ; i < n ; ++i){
        ans += abs((s[i]-rs[i])-(t[i]-rt[i])) ;
        p[i] = rt[a[i]];
    }

    mergeS(0, n-1, p) ;
    ans += inv ;
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
    // cin >> tc ;

    while(tc--){
        solve();
    }
    auto end = high_resolution_clock::now();
    duration<double> diff = end - start;
    cerr << fixed << setprecision(9) << diff.count() << "\n";
    return 0;
}