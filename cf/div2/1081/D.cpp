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
     int n ; cin >> n ;  
     vector<int> v(n) ; read(v) ;  
    vector<vector<int>> adj(n) ;  
    for(int i = 0 ; i+1<n ; ++i){
    	int u , v ; cin >> u >> v ; 
    	u-- ; v-- ;  
    	adj[u].pb(v) ;  
    	adj[v].pb(u) ;   
    }
    vector<int> c(n) , s(n) ;  
    vector<int> ans(n) ;  
    auto dfs = [&](auto &&self, int i ,int par)->int{ //return max_dpth
    
    	int subtree_sum = v[i] ;  
    	int C = 0 ; 
    	
    	vector<pair<int,int>> child ;  
    	int mxd = 1 ;  
    	for(auto &x:adj[i]){
    		if( x == par ) continue ;
    		
    		int d = self(self,x,i) ; 
    		child.pb({x,d}) ; 
    		mxd = max(mxd , d + 1 ) ;     
    		int cost = c[x] ;   
    		int tree_sum = s[x] ; 
    		subtree_sum += tree_sum ;  
    		C += tree_sum + cost ;  
    	}
    	ans[i] = C ;
	int m = child.size() ; 
	if( m > 1 ){
	    	vector<int> pmx_d(m) , smx_d(m) ;   
	    	pmx_d[0] = child[0].second ;   
	    	smx_d[m-1] = child[m-1].second ;  
	    	for(int j = 1 ; j<m ; ++j){
	    		pmx_d[j] = max(pmx_d[j-1],child[j].second) ;  
	    	}
	    	for(int j = m-2 ; j>=0 ; --j){
	    		smx_d[j] = max(smx_d[j+1],child[j].second)  ;  
	    	}
	    	for(int j = 0 ; j<m ; ++j){
	    		int pref = j>0?pmx_d[j-1]:0 ;  
	    		int suff = j+1<m?smx_d[j+1]:0 ;   
	    		int remaining = max(pref,suff) ;   
	    		ans[i] = max(ans[i], C + s[child[j].first]*remaining) ;  
	    	}
    	}
    	for(auto &x:adj[i]){
    		if( x == par ) continue ;
    		ans[i] = max(ans[i] , C - c[x] + ans[x]) ; 
    	}
    	c[i] = C ;   
    	s[i] = subtree_sum ;  
    	return  mxd ; 
    }; 
    
    dfs(dfs,0,-1) ;   
    show(ans,0) ;  
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



