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
void solve(){
    int n , k ; cin >> n >> k ;  
    string s ; cin >> s ;  
    vector<int> cnt(n)  ;  
    for(int i = 0 ; i<n ; ++i){
        if(s[i]=='o')cnt[i] = 1 ;  
        if(i>0)cnt[i]+=cnt[i-1] ;
    }
    vector<int> mpp(n+1,INF) ;   
    // cnt[r] - cnt[l-1] >= k  
    for(int i = 0 ; i<n ; ++i){
        mpp[cnt[i]] = min(mpp[cnt[i]],i) ;
    }

    double ans = 0 , l = 0 , h = 1 ;  
    while(h-l>1e-7){
        double x = (l+h)/2 ;  

        bool ok = false ; 

        vector<double> P(n) ;  
        for(int i = 0 ; i<n ; ++i){
            P[i] = cnt[i] - x*i ; 
        }
        vector<double> suffmax(n,-INF) ;
        suffmax[n-1] = P[n-1] ;
        for(int i = n-2 ; i>=0 ; --i){
            suffmax[i] = max(suffmax[i+1],P[i]) ;
        }

        for(int i = 0; i<n ; ++i){
            // cnt[r] - cnt[l-1] >= k
            // cnt[r] >= k + cnt[l-1]
            int val = k + (i>0?cnt[i-1]:0) ;
            if(val<=n){
                int minR = mpp[val] ; 
                if(minR>=i && minR<n){
                    // cnt[r] - x*r >= cnt[l-1] - x*(l-1)
                    // if l = 0  
                    // cnt[-1] = 0 as no entty and - x*(l-1) becomes -x*(-1) = x
                    if(suffmax[minR]-(i>0?P[i-1]:x)>=0){
                        ok = true ; 
                        break ; 
                    }
                }
            }
        }
        if(ok){
            ans = x ; 
            l = x ;
        }
        else{
            h = x ;
        }
    }
    
    sputdouble(ans) ;
    cout<<"\n" ; 
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