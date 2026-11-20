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
A move from u to v adds v minus u
The gcd of u and the increment equals the gcd of u and v
so a reaches b in 0 moves if they are equal and in 1 move if they are coprime

For b <= 100019 the sieve gives the largest prime p below b
If p > a then p is coprime to a
Bertrand gives p > b/2 so the only positive multiple of p up to b is p itself
The two increments are p minus a and b minus p

For b > 100019 use either 100003 or 100019 as p
Both primes exceed a and their product exceeds 1e9
so at least one is coprime to b and gives the same two moves

If there is no prime between a and b then the interval lies in a small prime gap
Let dp[i] be the minimum number of moves to reach a+i
Start with dp[0] = 0 and take dp[i] = 1 + min dp[j] over 0 <= j < i
such that a+j and a+i are coprime
Store the parent to recover the increments
*/

const int N = 1'000'19;
int prv[N+1] ;

void sieve(){
    vector<bool> prime(N+1,1) ;
    for(int i = 2 ; i*i <= N ; ++i){
        if(prime[i]){
            for(int j = i*i ; j <= N ; j += i){
                prime[j] = 0 ;
            }
        }
    }
    int p = 2 ;
    for(int i = 3; i <=N ; ++i){
        prv[i] = p ; 
        if(prime[i]) p = i ;
    }
}
const int p1 = 1'0000'3 ; 
const int p2 = 1'000'19 ; 

void solve(){
    int a, b ; cin >> a >> b ;
    if(a==b){
        put(0) ;
        return ;
    }
    if(__gcd(a,b)==1){
        put(1) ;
        put(b-a) ;
        return ;
    }
    if(b>p2){
        int p = b%p1==0 ? p2 : p1 ;
        put(2) ;
        cout<<p-a<<" "<<b-p<<"\n" ;
        return ;
    }
    if(prv[b]>a){
        int p = prv[b] ;
        put(2) ;
        cout<<p-a<<" "<<b-p<<"\n" ;
        return ;
    }

    vector<int> dp(b-a+1,INF) ;
    vector<int> parent(b-a+1,-1) ;
    // dp[i] = min moves to reach a+i
    dp[0] = 0 ;
    for(int v = a+1 ; v<=b ; ++v){
        for(int u = a ; u<v ; ++u){
            if(__gcd(u,v)==1 && dp[u-a]+1<dp[v-a]){
                dp[v-a] = dp[u-a]+1 ;
                parent[v-a] = u ;
            }
        }
    }

    vector<int> ans ;
    int curr = b ;
    while(curr != a){
        int prev = parent[curr-a] ;
        ans.push_back(curr-prev) ;
        curr = prev ;
    }
    reverse(all(ans)) ;
    put(ans.size()) ;
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
    sieve() ;
    while(tc--){
        solve();
    }
    auto end = high_resolution_clock::now();
    duration<double> diff = end - start;
    cerr << fixed << setprecision(9) << diff.count() << "\n";
    return 0;
}