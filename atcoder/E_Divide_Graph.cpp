#include <bits/stdc++.h>
#pragma GCC optimize("O3", "unroll-loops")
#define int long long
#define ll long long
#define eb emplace_back
using namespace std;
int mod = 998244353;
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
Any cut that avoids cutting a heavier edge will always be cheaper than a cut that includes it, regardless of how many smaller edges are involved.
since  

	2^i > 2^(i-1) + 2^(i-2) + ... + 2^2 + 2 + 1  

consider processing from right to left   

let S be the optimal solution 
and let S' be another solution such that the removed edges in S' and S match from [i+1 ... m ]    
and i be the first position it differs
let S have ith edge removed and S' doesnt 
then due to above fact S cant be cheaper than S' hence S is not optimal 
consider otherwise that S' has it removed and S doesnt 
due to same reason S' cant be optimal 
hence if ith can be kept then it should be kept 
bcz even though we keep all lower they cant nullify the loss of removing the ith edge 
instead if we had kept ith edge and removed every smaller one this would have costed us less than the prior idea




*/
class dsu{
    public:
    vector<int> parent,rank , mx ;
    int cnt ;  

    dsu(int n ){
    	cnt = n  ;  
        parent = vector<int>(n);
        mx = vector<int>(n);

        iota(parent.begin(),parent.end(),0) ;
        iota(mx.begin(),mx.end(),0) ;
        rank = vector<int>(n, 1);

    }
    void make_set(int v){
        parent[v] = v ; 
    }
    void union_sets(int a , int b){
        a = find_set(a) ;  
        b = find_set(b) ; 
        if(a!=b){
            if(rank[a]<rank[b])swap(a,b) ; 
            parent[b] = a  ;
            mx[a] = max(mx[a],mx[b]) ;
            if(rank[a]==rank[b])rank[a]++ ; 
            cnt-- ;  
        }
    }
    int find_set(int v){
        if (parent[v] != v)parent[v] = find_set(parent[v]); 
        return parent[v];
    }
};

void solve(){
     int n , m ; cin >> n >> m ;   
     vector<Edge> v(m) ;   
     for(int i = 0 ; i< m ; ++i){
     	int x, y  ; cin >> x >> y ;  
     	x-- ; y-- ;  
     	v[i] = {x,y,i+1} ;  
     }
     
     dsu d(n) ;  
     vector<int> skip ; 
     for(int i = m-1 ; i>=0 ; --i){
     	int x = v[i].u  ;  
     	int y = v[i].v  ;
     	if(d.cnt>2){
     		d.union_sets(x,y) ; 
     	}else if(d.find_set(x)!=d.find_set(y)) {
     		skip.pb(i+1) ;  
     	}
     }
     int ans = 0 ; 
     for(int &x:skip){
     	 ans = (ans + exp(2,x,mod))%mod ; 
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



