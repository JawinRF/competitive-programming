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
to get Left of L water i guess we can use prev greater element

using stack

let for index i prev greater be at index j then 

then l[j] + (h-a[i])*(i-j)
if PGE = -1 then l[j] = 0 
L[0] = h - a[0]

the same can be done for R
next greater 
for i let j be the NGE
=> R[j] + (h-a[i])*(j-i)
if NGE = n then R[j] = 0 ;
R[0] = h - a[n-1]

the water between i and j = L[i] + R[j] + middle(i+1,j-1) where middle(i+1,j-1)==0 for j<=i+1
for other case 
we can advance j incrementally 

we from j to j+1 we can find the PGE of a[j]
we can instead use the mid[j] = mid[PGE[j]] + (h - a[j])*(j-PGE[j]) ;
but PGE(j)<i then mid[j] = R[i] - R[j] - (h - a[i]) + (h - a[j]) ; 
if PGE(j)==i then mid[j] = (h - a[j]) * (j - i) ;
*/
void solve(){
    int n , h ; cin >> n >> h ; 
    vector<int> a(n) ; read(a) ;
    vector<int> NGE(n) ,PGE(n) , L(n) , R(n) ;
    stack<int> st ;
    for(int i = 0 ; i < n ; ++i){
        while(!st.empty() && a[st.top()] <= a[i]) st.pop() ;
        if(st.empty()) PGE[i] = -1;
        else PGE[i] = st.top() ;
        L[i] = (h - a[i])*(i-PGE[i]) + (PGE[i] == -1 ? 0 : L[PGE[i]]) ;
        st.push(i) ;
    }
    while(!st.empty()) st.pop() ;
    for(int i = n-1 ; i >= 0 ; --i){
        while(!st.empty() && a[st.top()] <= a[i]) st.pop() ;
        if(st.empty()) NGE[i] = n;
        else NGE[i] = st.top() ;
        R[i] = (h - a[i])*(NGE[i]-i) + (NGE[i] == n ? 0 : R[NGE[i]]) ;
        st.push(i) ;
    }
    int mx = 0 ;
    for(int i = 0 ; i < n ; ++i){
        mx = max(mx , L[i] + R[i] - (h - a[i])) ;
        vector<int> mid(n) ;
        for(int j = i+1 ; j < n ; ++j){
           if(PGE[j]>i){
                mid[j] = mid[PGE[j]] + (h - a[j])*(j-PGE[j]) ;
                mx = max(mx,mid[j] + L[i] + R[j] - (h - a[j])) ;
           }
           else if(PGE[j]==i){
                mid[j] = (h - a[j]) * (j - i) ;
           }
           else if(PGE[j]<i){
                mid[j] = R[i] - R[j] - (h - a[i]) + (h - a[j]) ;
           }
        }
    }
    put(mx) ;
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