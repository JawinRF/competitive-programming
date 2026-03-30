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
inline int ceil_div(int a, int b){ assert(b > 0); return (a + b - 1) / b; }
inline int floor_div(int a, int b){ assert(b > 0); return a / b; }
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
const int N = 2e5 ;  
bool in[N]  ;  
int n , a , b ;  
vector<vector<int>> adj ; 
/*
   // find path between pA and pB
*/
int d[N] ; 
void solve(){
	cin >> n  ;adj = vector<vector<int>> (n);    
	rep(i,n-1){
		in[i] = 0 ;  
		d[i] = 0  ; 
	}
	cin >> a >> b ; a-- ; b-- ;  
	rep(i,n-2){
		int u , v ; cin >> u >> v ; u-- ; v-- ;  
		adj[u].pb(v) ; adj[v].pb(u) ;  
	}
	auto dfs = [&](auto &&self , int node ,int par)->bool{
		if(node == b ){
			in[node] = 1 ;  
			return 1 ;  
		}  
		for(auto &x:adj[node]){
			if(x==par)continue ; 
			if(self(self,x,node)){
				in[node] = 1; 
				break ; 
			}	 
		}
		
		return in[node]  ;    
	} ;
	dfs(dfs,a,-1)  ;
	int pathlen = max(0LL,(ll)count(in , in+n ,1 ) - 1 )  ;  
	int c = ceil_div(pathlen,2) ;  
	
	int prev1 = -1 , prev2 = -1 ; 
	int p1 = a, p2 = b ; 
	for(int i = 0 ;  i<c  ;  ++i){
		for(auto &x:adj[p1]){
			if(x!=prev1 && in[x]){
			    prev1 = p1;
			    p1 = x;
			    break;
			}
		}
		for(auto &x:adj[p2]){
			if(x!=prev2 && in[x]){
			    prev2 = p2;
			    p2 = x;
			    break;
			}
		}
	}
	auto f = [&](auto &&self, int i ,int par ,int depth )->void{  
		d[i] = depth ; 
		for(auto &x:adj[i]){
			if( x == par ) continue ; 
			self(self,x,i,depth+1) ;   
			d[i] = max(d[i] , d[x]) ; 
		}
	};
	f(f,p2,-1, 0) ;  
	
	int cnt = 1 , Cost = 0 ; 
  
	auto color = [&](auto &&self, int i  , int par ) -> void {
		
		sort(all(adj[i]), [&](int u , int v){
			return d[u]<d[v] ;  
		}) ; 
		for(auto &x:adj[i]){
			if(x==par)continue ;  
			cnt++ ; 
			Cost++ ;  
			self(self,x,i) ; 
			if(cnt!=n)Cost++ ;     
		}
	} ;  
	color(color,p2,-1) ;  
	Cost = Cost + c; 
	put(Cost)  ; 
		
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



