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
vector<int> t , b;  
vector<vector<int>> dp; 
int f(int u , int d){
    
    if(u==-1 && d==-1)return 0 ; 
    if(min(u,d)<-1)return INT_MIN ;  
    
    if(dp[u+1][d+1]!=-1){
        return dp[u+1][d+1] ;
    }
    int mx = 0 ; 
   
    // case 1 
    if(d>=2)mx  = max(f(u,d-3) + ((b[d] + b[d-1] + b[d-2]) >= 2),mx) ;
    
    if(u>=2) mx = max(f(u-3,d) + ((t[u] + t[u-1] + t[u-2]) >= 2), mx);

    // case 2
    if(u>=d-1 && (u-(d-1))%3==0){
        int score = 0 ;  
        for(int i = u ; i > d-1 ; i-=3){
            score += (t[i] + t[i-1] + t[i-2])>=2 ;
        }
        mx = max(mx,f(d-2,d-2) +  score + ((b[d]+b[d-1]+t[d-1])>=2)) ; 
    }
    
    if(d>=u-1 && (d-(u-1))%3==0){
        int score = 0 ;  
        for(int i = d ; i > u-1 ; i-=3){
            score += (b[i] + b[i-1] + b[i-2])>=2 ;
        }
        mx = max(mx,f(u-2,u-2) + score + ((t[u]+t[u-1]+b[u-1])>=2)); 
    }
    
    // case 3
    if(u>=d && (u-d)%3==0){
        int score = 0 ;  
        for(int i = u ; i > d ; i-=3){
            score += (t[i] + t[i-1] + t[i-2])>=2 ;
        }
        mx = max(mx,f(d-1,d-2) +  score + ((b[d]+b[d-1]+t[d])>=2)) ; 
    }
    if(d>=u && (d-u)%3==0){
        int score = 0 ;  
        for(int i = d ; i > u ; i-=3){
            score += (b[i] + b[i-1] + b[i-2])>=2 ;
        }
        mx = max(mx,f(u-2,u-1) + score +  ((t[u]+t[u-1]+b[u])>=2)); 
    }

    return dp[u+1][d+1] = mx ; 
   
}
void solve(){
    int n ; cin >> n ;   
    b = vector<int>(n) ; t = vector<int>(n) ; 
   
    dp  = vector<vector<int>> (n+1,vector<int> (n+1,-1)) ; 
    // A -> 1 , J ->  0 

    for(auto &x:t){
        char c ; cin >> c ; x = (c=='A' ? 1 : 0) ;
    }
    for(auto &x:b){
        char c ; cin >> c ; x = (c=='A' ? 1 : 0) ;
    }
    cout<<f(n-1,n-1) ;   
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
    cin >> tc ;

    while(tc--){
        solve();
    }
    auto end = high_resolution_clock::now();
    duration<double> diff = end - start;
    cerr << fixed << setprecision(9) << diff.count() << "\n";
    return 0;
}


