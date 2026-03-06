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
 A*s[n-1] + B*s[n-2] mod M   
 
 (A mod M * s[n-1] mod M + B mod M * S[n-2] mod M ) mod M 
 
 ( A mod M * k' + B mod M * k'' mod M ) mod M 
 the next state is solely dependent on 
 
 (k',k'') both being in [ 0, M-1]  

since we dont wanna encounter a zero  

since there are at most M^2 such combiinations of k' , k''


if current state is ( k' , k'') then next stage is 
( k'' , ( A*k' + B*k'') mod M )

essentaully its a sliding window of size 

we can pre-construct graphs for each distinct initial sliding window of size 2	

for a particular ( k' , k'') 
 ( k'' , k''') must already been known  
 
*/
int M[1000][1000] ;  
void solve(){
     int m , a , b ; cin >> m >> a >> b ;  
     vector<vector<vector<pair<int,int>>>> adj(m,vector<vector<pair<int,int>>> (m)) ; 
     for(int i =  0 ; i<m ; ++i){
     	for(int j = 0 ; j<m ; ++j){
     		// [i,j] -> [j , A*j + B*i ]  
     		adj[j][(a*j + b*i)%m].pb({i,j}) ;  
     	}
     }
     
     // M[i][j] is marked true if its reachable from (0,x)  
     queue<pair<int,int>> q;   
     for(int i = 0 ; i<m ; ++i){
     	q.push({0,i}) ;  
     	M[0][i] = 1 ;  
     }
     while(!q.empty()){
     	auto [x,y] = q.front() ;  
     	q.pop() ; 
     	for(auto &p:adj[x][y]){
     		if(M[p.first][p.second])continue ;
     		M[p.first][p.second] = 1 ; 
     		q.push(p) ;  
     	}
     }
     int ans = m*m ;   
     for(int i = 0;  i <m ; ++i){
     	for(int j = 0 ;j<m ; ++j){
     		ans -= M[i][j] ;  
     	}
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
    //cin >> tc ;

    while(tc--){
        solve();
    }
    auto end = high_resolution_clock::now();
    duration<double> diff = end - start;
    cerr << fixed << setprecision(9) << diff.count() << "\n";
    return 0;
}



