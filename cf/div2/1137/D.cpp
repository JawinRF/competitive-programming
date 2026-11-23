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
operation is defined for i>=2
let p[i] = a[1] + a[2] + ... + a[i] 
p[i] + a[i] -a[i] -a[i] 
p[i] - a[i]  = p[i-1]

p[i-1] + a[i] = p[i] 

p[i+1] + a[i] -a[i] -a[i] +a[i]  = p[i+1] 

so the operation at i just swaps p[i] and p[i-1]

consider the final array to be b
b[i] = p'[i] - p'[i-1] 
to have b[i] >= 0 
we have p'[i] >= p'[i-1]
so essentially we want p to be nondecreasing
SO the problem is min No of adjacent swaps to sort p . 

min no of adjacent swaps is just count inversion
the only way we get -1 is to have p[1] < 0 
*/
int inv ;
vector<int> count_inversions(vector<int> &p, int l, int r) {
    if (l >= r) return {p[l]} ;
    int mid = (l + r)/2;
    vector<int> r1 = count_inversions(p, l, mid) , r2 = count_inversions(p, mid + 1, r);
    
    vector<int> res ;
    int i = 0, j = 0;
    while(i<r1.size() && j<r2.size()) {
        if (r1[i]<=r2[j]) {
            res.push_back(r1[i++]);
        }else{
            res.push_back(r2[j++]);
            inv += (r1.size() - i);
        }
    }
    while(i < r1.size()) {
        res.push_back(r1[i++]);
    }
    while(j < r2.size()) {
        res.push_back(r2[j++]);
    }
    
    return res ; 
}
void solve(){
    int n ; cin >> n ; 
    vector<int> v(n) ; read(v) ; 
    vector<int> p(n) ;
    p[0] = v[0] ;
    for(int i = 1 ; i<n ; ++i){
        p[i] = p[i-1] + v[i] ;
    }
    inv = 0 ;
    p = count_inversions(p,0,n-1) ;
    if(p[0]<0){
        put(-1) ;
        return ;
    }
    put(inv) ;
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