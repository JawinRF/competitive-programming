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
 if only -1 and +1
  since value starts from 0  and there are no jumps if sum = X is possible then sum = X-1 is also possible
  => output  [ min subaaray sum , max subarray sum ]
  
  else
  calculate above for both [....,i-1] and [i+1,...]
  now
   
  [ i , .... ]
  start is fixed so increment end and hash it
  
  [ ... , i ]
  lly end is fixed
  
  [ .... , i , ... ] 
  
*/


pair<int, int> f(vector<int>& a,int l , int r) {
    int max_sum = 0,cur_max = 0 ,min_sum = 0 , cur_min = 0;
    for (int i = l ; i<=r ; ++i) {
    	int x = a[i] ; 
        cur_max += x;
        if (cur_max < 0) cur_max = 0;
        max_sum = max(max_sum, cur_max);
        cur_min += x;
        if (cur_min > 0) cur_min = 0;
        min_sum = min(min_sum, cur_min);
    }
    return {min_sum, max_sum};
}
void p(vector<int> &a,int mn, int mx){
	for(int i = mn ; i<=mx ; ++i)a.pb(i) ;  
}
void solve(){
     int n ; cin >> n ;   
     vector<int> v(n) ;  read(v) ;   
     int j = -1 ;  
     for(int i = 0 ; i<n ; ++i){
     	if(v[i]!=1 && v[i]!=-1)j =i  ;  
     }
     
     vector<int> u ; 
     pairii r  ;
     if(j<n-1){
	     r = f(v,j+1,n-1) ;   
	     p(u,r.first,r.second) ;  
      } 
     
     if(j>0){
        r = f(v,0,j-1) ;   
	p(u,r.first,r.second) ; 
     }
     if(j!=-1){
        int c_mn = v[j] ,c_mx = v[j];   
        int s = 0 ; 
        for(int i = j ; i<n ; ++i){
            s += v[i] ;   
            c_mn = min(c_mn , s) ;  
            c_mx = max(c_mx,s) ;  
        }  
     	p(u,c_mn,c_mx) ;
     	
     	c_mn = v[j] ;c_mx = v[j]; s = 0 ; 
        for(int i = j ; i>=0 ; --i){
            s += v[i] ;   
            c_mn = min(c_mn , s) ;  
            c_mx = max(c_mx,s) ;  
        }
     	 
     	p(u,c_mn,c_mx) ;
     	
     	int mn = c_mn , mx = c_mx  ;  
     	int curr_mn = mn , curr_mx = mx ; 
     	for(int i = j+1 ; i<n ; ++i){
     		if(v[i]==-1){
     			mn-- ;  
     			mx-- ;   
     		}
     		else{
     			mn++ ;  
     			mx++ ;  
     		}
     		curr_mn = min(curr_mn,mn)  ;  
     		curr_mx = max(curr_mx ,mx) ;  
     	}
     	p(u,curr_mn,curr_mx) ;
     }
     u.pb(0) ; 
     unique(u) ; 
     put(u.size()) ;  
     show(u,0) ; 
     
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



