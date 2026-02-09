#include <bits/stdc++.h>
#pragma GCC optimize("O3", "unroll-loops")
#define int long long

#define ll long long
#define eb emplace_back
using namespace std;
int mod = 998244353 ;
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
ll exp(ll x, ll n, ll m) {
    assert(n >= 0);
    x %= m;
    ll res = 1;
    while (n > 0) {
        if (n % 2 == 1) {
            res = res * x % m;
        }
        x = x * x % m;
        n /= 2;
    }
    return res;
}

int extended_gcd(int a, int b, int &x, int &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    int x1, y1;
    long long gcd = extended_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return gcd;
}

int modinv(int a, int m) {
    int x, y;
    int gcd = extended_gcd(a, m, x, y);
    if (gcd != 1) return -1;
    return (x % m + m) % m;
}


void unique(vector<int> &b) {
    sort(b.begin(), b.end());
    auto it = unique(b.begin(), b.end());
    b.erase(it, b.end());
}

vector<int> sieve(int n) {
    vector<int> smallest_prime_factor(n+1);
    for (int i = 2; i * i <= n; ++i) {
        if (!smallest_prime_factor[i]) {
            for (int j = i * i; j <= n; j += i)
                smallest_prime_factor[j] = i;
        }
    }
    vector<int> primes;
    for (int i = 2; i <= n; ++i) {
        if (smallest_prime_factor[i] == 0){
            smallest_prime_factor[i] = i;
            primes.push_back(i);
        }
    }
    return primes ;
}





struct custom_hash {
    static uint64_t fixed_random;

    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }
    template <class T>
    size_t operator()(T x) const {
        return splitmix64(x + fixed_random);
    }

    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);

        uint64_t mixed1 = splitmix64(h1 + fixed_random);
        uint64_t mixed2 = splitmix64(h2 + fixed_random);

        return mixed1 ^ (mixed2 >> 1);
    }
};

uint64_t custom_hash::fixed_random = std::chrono::steady_clock::now().time_since_epoch().count();

int addmod(int a, int b) {
    assert((a + b) < 2 * mod);
    a += b;
    if (a >= mod) a -= mod;
    return a;
}
struct Edge {
    int u, v, w;
};

const int INF = 1e18;
vector<vector<int>> matrixMul(vector<vector<int>> &A,vector<vector<int>> &B,int n){
    vector<vector<int>> res(n,vector<int> (n)) ;
    for(int i = 0 ;  i<n ;  ++i){
        for(int j = 0 ; j<n ; ++j){
            for(int k = 0 ; k<n ; ++k){
                res[i][j] = (res[i][j] + A[i][k]*B[k][j] + mod)%mod ;
            }
        }
    }
    return  res ;
}

vector<vector<int>> matrixExponentiation2D(vector<vector<int>> &x,int power){
    if(power==0){
        return {{1, 0}, {0, 1}}; // identity matrix
    }
    if(power==1)return x;
    vector<vector<int>> res = matrixExponentiation2D(x,power/2) ;
    res = matrixMul(res,res,x.size()) ;
    if(power%2==1){
        res = matrixMul(res,x,x.size()) ;
    }
    return res ;
}
vector<vector<int>> matrixExponentiation(vector<vector<int>> &x,int power){
    if(power==0){
        return {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}; // identity matrix
    }
    if(power==1)return x;
    vector<vector<int>> res = matrixExponentiation(x,power/2) ;
    res = matrixMul(res,res,x.size()) ;
    if(power%2==1){
        res = matrixMul(res,x,x.size()) ;
    }
    return res ;
}
int dx[] = {-1,1,0,0} ;
int dy[] = {0,0,-1,1} ;
template <typename T, typename U>
ostream& operator<<(ostream& os, const pair<T, U>& p) {
    os << p.first << " " << p.second << "\n" ;
    return os;
}

pair<int,int> intersection(pair<int,int> &p1,pair<int,int> &p2){
    return {max(p1.first,p2.first),min(p1.second,p2.second)};
}
int ceil_div(int a, int b){
    assert(b>0) ;
    return (a+b-1)/b;
}
int floor_div(int a, int b){
    assert(b>0) ;
    return a/b;
}
template<typename U>
void matrixTranspose(vector<vector<U>> &matrix){
    int n = matrix.size() ;
    int m  = matrix[0].size() ;
    if (n == m) {
        for(int i = 0 ; i < n ; ++i){
            for(int j = i+1 ; j < m ; ++j ){
                swap(matrix[i][j], matrix[j][i]) ;
            }
        }
    }
    else{
        vector<vector<U>> temp(m,vector<U>(n)) ;
        for(int i  = 0 ; i < n ; ++i){
            for (int j = 0 ; j < m ; ++j){
                temp[j][i] = matrix[i][j] ;
            }
        }
        matrix = move(temp) ;
    }
}
const int mxm = 2e5 + 1 ;

int distance(int x, int y , int n ) {
     if (y>x){
        return y-x ;
     }
     return n - x + y  ; // got till the end and then wrap around
}
int MSB(int p){
    return 63 - __builtin_clzll(p) ;
}
vector<int> distance(int node,int n,vector<vector<int>> &adj){
    queue<int> q ; q.push(node) ;
    vector<int> dist(n,INT_MAX) ;
    dist[node] = 0 ;
    while(!q.empty()){
       int curr = q.front() ;
       q.pop() ;
       for(auto &x:adj[curr]){
          if(dist[x]<=dist[curr]+1)continue;
          dist[x] = dist[curr]+1 ;
          q.push(x) ;
      }
    }
    return dist ;
}
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
    void union_sets(int a , int b){
        a = find_set(a) ;  
        b = find_set(b) ; 
        if(a!=b){
            if(rank[a]<rank[b])swap(a,b) ; 
            parent[b] = a  ;
            mx[a] = max(mx[a],mx[b]) ;
            if(rank[a]==rank[b])rank[a]++ ; 
        }
    }
    int find_set(int v){
        if (parent[v] != v)parent[v] = find_set(parent[v]); 
        return parent[v];
    }
};

void solve(){
    int n , m , d ; cin >> n >> m >> d ; 
    vector<vector<bool>> v(n,vector<bool> (m)) ;   
    
    for(int i = 0 ; i<n ; ++i){
        for(int j = 0 ; j<m ; ++j ){
            char c ; cin >> c ;  
            if(c == 'X')v[i][j] = 1 ;  
        }
    }
    vector<int> p12(m),dp0_vertical(m),pref0(m);
    
    for(int j = 0; j < m; ++j) {
        if(v[0][j]) dp0_vertical[j] = 1;
        pref0[j] = dp0_vertical[j] + (j>0?pref0[j-1]:0LL);
        pref0[j] %= mod;
    }
    
    for(int j = 0; j < m; ++j) {
        if(!v[0][j]) continue;
        int ways = dp0_vertical[j];

        int l = max(0LL,j-d);
        int r = min(m-1,j+d);
        
        int h = pref0[r] - (l>0?pref0[l-1]:0LL);
        h = (h%mod + mod)%mod;
        
        h = (h-dp0_vertical[j]+mod)%mod;
        
        p12[j] = (ways + h)%mod;
    }
    int max_dist = sqrt(d*d-1) ;  
    for(int j = 1; j < m; ++j) {
        p12[j] = (p12[j] + p12[j-1])%mod;
    }
    for(int i = 1 ; i<n ; ++i ) { 
        vector<int> dp(m) ; 
        vector<int> curr(m) ; 
        for(int j = 0 ; j< m  ;++j){
            if(!v[i][j])continue ; 
            
            // step 1 dp[i][j][1] += dp[i-1][y][2] + dp[i-1][y][1] 
            int l = max(0LL,j-max_dist)  ;  
            int r = min(m-1,j+max_dist) ;  
            int dp1 = ((p12[r] - (l>0?p12[l-1]:0LL))%mod + mod)%mod ; 
            dp[j] = dp1 ;  
            curr[j] = dp1  ; 
        }
        
        for(int j = 1 ; j<m ; ++j ) {  
            curr[j] = (( curr[j] + curr[j-1])%mod + mod)%mod ;   
        }
        
        // Step2  (i,j)[2] += (i,j')[1]  
        for(int j = 0 ; j<m ; ++j){
            if(!v[i][j])continue ;  
            int l = max(0LL,j-d) ;  
            int r = min(m-1 , j+d ) ;  
            
            int dp2 = ((curr[r] - (l>0?curr[l-1]:0LL))%mod + mod)%mod  ;  
            dp2 = ((dp2 - dp[j])%mod + mod )%mod;  
            dp[j] = (dp[j]+dp2)%mod ; 
        }
        
        for(int j = 1; j<m; ++j)dp[j] = (( dp[j]+ dp[j-1] )%mod + mod)%mod ; 
        swap(p12,dp) ; 
    }
    int ans = p12[m-1] ;  
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

    while(tc--){
        solve();
    }
    auto end = high_resolution_clock::now();
    duration<double> diff = end - start;
    cerr << fixed << setprecision(9) << diff.count() << "\n";
    return 0;
}