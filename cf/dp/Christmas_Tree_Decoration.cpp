#include <bits/stdc++.h>
#pragma GCC optimize("O3", "unroll-loops")
#define int long long
#define ll long long
#define eb emplace_back
using namespace std;
int mod = 998244353 ; 
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
    turns  = floor(S/n)  
    extra = S mod n  = r 
    if number of turns for i < a[i] then not possible 
    so a[i]<=no of turns 
    if a[i] == turns+1 then this must be part of the 'extra' group 
    so a[i] > turns+1 impossible 
    
    we have 'extra' followed by 'normal' grp  
    the normal grp can be permuted anyway ( n - r )! 
    we need to fill remaining spots in 'extra' by any other element 
    let cnt of elements with a[i] == turns+1 be 'c' 
    then (n-c) C ( r-c ) * r! is arrangement 
    
    1/x mod = x^p-1 / x mod = x^p-2 mod 
    
    
    
*/
int fact[51]  ;  
int power(int b, int e){
	int res = 1 ;  
	b %= mod ;   
	while(e>0){
		if(e%2==1)res = (res*b)%mod ;   
		b = (b*b)%mod ;  
		e /= 2 ;   
	}
	return res ;  
}
int modInverse(int n){
	return power(n,mod-2) ;  
}
int nCr(int n , int r){
	return fact[n]*modInverse(fact[n-r])%mod*modInverse(fact[r])%mod ;  
}
void solve(){
     int n ; cin >> n ;  
     vector<int> v(n+1) ;  
     read(v) ;  
     int S = getsum(v) ;   
     int q = floor_div(S,n) ;  
     int r = S%n ;   
     
     int c =  0 ;  
     for(int i = 1 ; i<=n ; ++i){
     	if(v[i]==q+1)c++ ;  
     	else if(v[i]>q+1){
     		put(0) ;   
     		return ; 
     	}
     }
     if(c>r){
     	put(0) ;   
     	return ;  
     }
    int ans = nCr(n-c,r-c);
    ans = (ans*fact[r])%mod;
    ans = (ans*fact[n-r])%mod ; 
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
    cin >> tc ;
    fact[0] = 1 ; 
    for(int i = 1 ; i<=50 ; ++i){
    	fact[i] = (i*fact[i-1])%mod ;  
    }
    while(tc--){
        solve();
    }
    auto end = high_resolution_clock::now();
    duration<double> diff = end - start;
    cerr << fixed << setprecision(9) << diff.count() << "\n";
    return 0;
}



