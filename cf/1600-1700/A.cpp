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
oh ya i dd it and it worked , also i didnt notice the p<q part of the problem . regardless my idea was 
its never worser for a element to face mod n as well as mod m compared to only facing a single of those .  
i did that using the idea that suppose we take mod q and we argue thats sufficient now mod q can be greater than p if so then taking mod p now will make sure we get even lower . if mod q is less than p then mod p wont effect anything thus achieving as much as what mod q gave .  WLOG it applies to mod p being first .  
let f(x) = x mod p mod q and g(x) =  x mod q mod p

so ideally each element wants to achieve min(f(v[i]) , g(v[i])) but how ? see the thing is for the first interval of length k that is selected we will decide which is better either sum(f(v[i])) or sum(g(v[i])) over i>=l and i<=r 
once done  . 
Now we look at cases
f(f(x)) it doesnt change anythin 
g(f(x))  = mod p mod q mod q mod p = mod p mod q mod p
if p<q then mod p mod q  mod p = mod p mod p = mod p = mod p mod q
if p>q then mod p mod q mod p  = mod p mod q 
so either way its same 
WLOG it applies to g(g(x)) and g(f(x)) 
so it means that after applying f subsequent operations being f or g dont change anything

extend our left right boundary . we select l = l-1 and r = r-1 and we can achieve min(f(v[i]) , g(v[i])) for i = l-1 keeping evrything intact 
lly we extend on both ways . 

Only thing to prove is showing 
mod p mod q or mod q mod p is as effective as just mod p or mod q for the first interval .   
suppose mod p was a optimal answer
then if p < q then mod p mod q = mod p 
if q < p then since mod p is optimal mod q > mod p => but since q<p mod q < mod p 
meaning mod p mod q  = mod q which is bad 
but then lly mod q mod  p 
gives if p<q mod q and q<p mod p 
so we get mod p if p<q and q<p so our statetgy is optimal .  

Can you walk through each line and check if I am right	
*/

//https://codeforces.com/problemset/problem/2215/A
void solve(){
     int n , k , p ,q  ;cin >> n>>k>>p>>q ;  
     vector<int> v(n) ;  
     read(v) ;  
     vector<int> pf(n) ;   
     for(int i = 0 ; i<n  ;++i){
     	pf[i] = min((v[i]%p)%q , (v[i]%q)%p);
     	if(i)pf[i] += pf[i-1] ; 
     }
     int modpq = 0 , modqp = 0 ;  
     for(int i = 0 ; i<k-1 ; ++i){
     	modpq += (v[i]%p)%q ; 
     	modqp += (v[i]%q)%p ; 
     }
     int ans = INF ; 
     for(int i = 0 ; i+k-1<n ; i++ ){
     	modpq += (v[i+k-1]%p)%q ; 
     	modqp += (v[i+k-1]%q)%p ; 
     	if(i){
     		modpq -= (v[i-1]%p)%q ; 
     		modqp -= (v[i-1]%q)%p ;
     	}
     	int l = i>0?pf[i-1]:0LL;  
     	int r = pf[n-1] - pf[i+k-1] ; 
     	ans = min(ans,l+r+min(modpq,modqp)) ; 
     }
     put(ans);
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



