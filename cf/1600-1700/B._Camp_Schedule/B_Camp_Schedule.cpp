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

struct StringHash{
    const int MOD = 1e9 + 9;
    int p ;
    vector<int> p_pow, h;

    StringHash(const string &s, int prime = 31){
        p = prime ;
        int n = (int)s.size() ; 
        p_pow.resize(n+1);
        h.resize(n+1);
        p_pow[0] = 1;
        for(int i = 0 ; i<n ; ++i){
            p_pow[i+1] = (p_pow[i] * p) % MOD ;
            h[i+1] = (h[i]*p + (s[i]-'0'+1))% MOD ;
        }
    }

    int get_hash(int l , int r){
        return (h[r+1] -h[l]*p_pow[r-l+1]%MOD + MOD ) % MOD ;
    }
};
void solve(){
    string s , t ; cin >> s >> t ; 
    int cS0 = count(all(s),'0') , cS1 = count(all(s),'1') ;
    int cT0 = count(all(t),'0') , cT1 = count(all(t),'1') ;
    if(cS0<cT0 || cS1<cT1){
        put(s) ; 
        return ;
    }

    if(t.length() == 1){
        put(s) ; 
        return ;
    }

    int lPS = 0 ;
    StringHash sh(t) ;
    StringHash sh2(t,92821) ;
    int n = t.length() ;
    for(int i = 0 ; i<n-1; ++i){
        if(sh.get_hash(0,i) == sh.get_hash(t.length()-1-i,t.length()-1) && sh2.get_hash(0,i) == sh2.get_hash(t.length()-1-i,t.length()-1)){
            lPS = i+1 ;
        }
    }

    string add = t.substr(lPS) ;
    cS0 -= cT0 ;
    cS1 -= cT1 ;
    int add0 = count(all(add),'0') , add1 = count(all(add),'1') ;
    string ans = t ; 
    while(1){
        if(cS0 >= add0 && cS1 >= add1){
            ans += add ;
            cS0 -= add0 ; 
            cS1 -= add1 ; 
        }else{
            break ;
        }
    }
    ans.append(cS0,'0') ;
    ans.append(cS1,'1') ;
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
