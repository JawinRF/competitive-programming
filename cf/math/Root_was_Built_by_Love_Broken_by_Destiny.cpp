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
the graph formed is connected
reason : you can travel from any house to any other by crossing bridges

the graph cant have cycles
Reason : 
cycle length odd then 2 vertices lying on the same side must have  a bridge
even length -> atleast one bridge will intersect 

=> The graph is a tree

A vertice cant have more than 2 non-one degree nodes 
reason   :  They will there will be intersection  

all single degree nodes connected to this vertice are forced to reside in between the non-one degree vertice
=> their arragements are restricted to that area 

the ans = 4 * Product ( d[i]-1)!
This assumes as atleast length of 2

for length 1
ans = 2

to get the length remove all one degree nodes 

edge case :  when the structure is a star
*/
class dsu{
    public:
    vector<int> parent,rank , mx ;

    dsu(int n ){
        parent = vector<int>(n);
        mx = vector<int>(n);

        iota(parent.begin(),parent.end(),0) ;
        iota(mx.begin(),mx.end(),0) ;
        rank = vector<int>(n, 1);

    }
    void make_set(int v){
        parent[v] = v ; 
    }
    bool union_sets(int a , int b){
        a = find_set(a) ;  
        b = find_set(b) ; 
        if(a!=b){
            if(rank[a]<rank[b])swap(a,b) ; 
            parent[b] = a  ;
            mx[a] = max(mx[a],mx[b]) ;
            if(rank[a]==rank[b])rank[a]++ ; 
            return true ; 
        }
        return false ; 
    }
    int find_set(int v){
        if (parent[v] != v)parent[v] = find_set(parent[v]); 
        return parent[v];
    }
};

void solve(){
     int n , m ; cin >> n >> m ;  
     vector<vector<int>> adj(n) ; 
     vector<bool> leaf(n,1) ;  
     dsu d(n) ; 
     bool bad = false ; 
     for(int i = 0 ; i<m ; ++i){
     	int a , b  ;cin >> a >> b ;a-- ; b-- ; 
     	adj[a].pb(b) ;adj[b].pb(a) ; 
     	leaf[a] = adj[a].size()<2 ;  
     	leaf[b] = adj[b].size()<2 ;
     	if(!d.union_sets(a,b)){
     		bad = true  ;
     	}
     }
     if(bad){
     	put(0) ;  
     	return ;  
     }
     
     vector<int> c(n) ;  
     for(int i  = 0 ; i<n ; ++i){
     	int cnt = 0 ;  
     	for(auto &x:adj[i]){
     		if(!leaf[x])cnt++ ;    
     	}
     	if(cnt>2){
     		put(0) ;  
     		return  ;  
     	}
     	c[i] = cnt ; 
     }
     int pathlen = count(all(leaf),0) ;  
     if(pathlen == 0 ){ 
     	put(2) ;  
     }
     else if(pathlen==1){
     	int ans = 2 ;  
     	for(int i = 1 ; i<n ; ++i){
     		ans = (ans*i)%mod ;   
     	}
     	put(ans);
     }
     else {
     	 vector<int> f(n+1,1);
         for(int i = 1; i <= n; ++i){
             f[i] = (f[i-1]*i)%mod;
         }
     	int ans = 4 ;  
     	for(int i = 0 ; i<n ; ++i){
     		ans = (ans*f[max(1LL,(ll)adj[i].size() - c[i])])%mod;   
     	}
     	put(ans) ;  
     }
    
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



