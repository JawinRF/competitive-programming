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
   The total cost doesnt exceed 2   
   reason  two even numbers have a gcd >=2 
   to make we need atmost 2 ops 
   
   cnt each prime factor of each element only once 
   => if a prime 'p' occurs more than once 2 elements have gcd >=p 
   
   This is enough for cost 0  
   
   we always have cost 2 as the last resort 
   remove existence of prime factors of current element  i from pool   
   increment it by 1 see if the incremented element's prime factors occur more than once 
   
   
*/



const int N = 2'00'005 ;  
int f[N+1]  , spf[N+1] , best[N+1] ;  
void sieve() {
    for (int i = 2; i <= N; ++i) {
        if (!spf[i]) {
             spf[i] = i ; 
            for (int j = i * i; j <= N; j += i)
                if(!spf[j])spf[j] = i;
        }
    }
} 
void solve(){
     int n ; cin >> n ;   
     vector<int> a(n),b(n)  ; read(a) ; read(b);
     bool f1 = 0 ;  
     for(int i = 0 ; i<n ; ++i){
     	int t = a[i] ;  
     	while(t>1){
     		int p = spf[t] ;  
     		while(t%p==0){
     			t /= p ;  
     		}
     		f[p]++ ;  
     		//cout<<p<<" " <<f[p]<<"\n" ; 
     		if(f[p]>=2){	
     			f1 = 1 ;   
     		}
     	}
     }
     int ans = INF ; 
     if(!f1){
     	for(int i = 0 ; i<n ; ++i){	
     		int t = a[i] ;  
     		// rem
     		while(t>1){
	     		int p = spf[t] ;  
	     		while(t%p==0){
	     			t /= p ;  
	     		}
	     		f[p]-- ; 
     		}
     		t = a[i]+1 ;  
     		while(t>1){
	     		int p = spf[t] ;  
	     		while(t%p==0){
	     			t /= p ;  
	     		}
	     		
	     		if(f[p]){
	     			ans = min(ans,b[i]) ; 
	     		} 
	     			 
     		}
     		//add back 
     		t = a[i] ; 
     		while(t>1){
	     		int p = spf[t] ;  
	     		while(t%p==0){
	     			t /= p ;  
	     		}
	     		f[p]++ ; 
     		}
     	}
     }
     for(int i = 0 ; i<n ; ++i){	
     	int t = a[i] ;  
     	// rem
	while(t>1){
     		int p = spf[t] ;  
     		while(t%p==0){
     			t /= p ;  
     		}
     		f[p]-- ; 
	}
	t = a[i]+1 ;  
	while(t>1){
     		int p = spf[t] ;  
     		while(t%p==0){
     			t /= p ;  
     		}
     		
     		if(f[p]){
     			ans = min(ans,b[i]+best[p]) ; 
     		} 
     		best[p] = min(best[p],b[i]) ; 
     		f[p]++ ; 
     			 
	}
     }
     
     // restore 
     for(int i = 0 ; i<n ; ++i){
     	int t = a[i]+1 ;  
     	while(t>1){
     		int p = spf[t] ;  
     		while(t%p==0){
     			t /= p ;  
     		}
     		f[p]-- ; 
     		best[p] = INF ; 
     	}
     }
     if(f1){
     	put(0);  
     }
     else put(ans) ; 
     	 
     
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
    sieve() ; 
    for(int i = 1 ; i<=N ; ++i)best[i] = INF ; 
    while(tc--){
        solve();
    }
    auto end = high_resolution_clock::now();
    duration<double> diff = end - start;
    cerr << fixed << setprecision(9) << diff.count() << "\n";
    return 0;
}



