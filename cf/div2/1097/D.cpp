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
c[i] = a[i]*b'[i] 
where b = permutation of chosen uniformly among all n!

inversion  = sum over all i of  sum over all j!=i of  X[i][j] 
X[i][j] = 1 if c[i] > c[j] and i<j
E[ inversion ] = sum over all i of  sum over all j>i of  E[X[i][j]] due to linearity of expectation
Now we know E[X[i][j]] = P(X[i][j]=1)*1 + P(X[i][j]=0)*0 
=> E[X[i][j]] = P(X[i][j] = 1)

So 
E[ inversion ] = sum over all i of  sum over all j>i of  P(X[i][j] = 1)

Now we need to find P(X[i][j] = 1)
P(X[i][j] = 1) = P(c[i] > c[j]) = P(a[i]*b'[i] > a[j]*b'[j])

there are n*(n-1) options for (b'[i] , b'[j]) and we need to find in how many of those options a[i]*b'[i] > a[j]*b'[j]

= 1/(n*(n-1)) * (number of options for (b'[i] , b'[j]) such that a[i]*b'[i] > a[j]*b'[j])
= 1/(n*(n-1)) * sum over all x,y and x!=y of I ( a[i]*x > a[j]*y ) where I is the indicator function that is 1 if the condition is true and 0 otherwise

so E[inversion] = sum over all i of  sum over all j>i of 1/(n*(n-1)) * sum over all x,y and x!=y of I ( a[i]*x > a[j]*y )

int n ;cin>>n ; 
vector<int> a(n),b(n) ; read(a) ;read(b) ; 
int ans = exp(n*(n-1),mod-2,mod) ; // 1/(n*(n-1)) mod m 
int count = 0 ;
for(int i = 0 ; i < n ; ++i){
    for(int j = i+1 ; j < n ; ++j){
        for(int x = 0 ; x < n ; ++x){
            for(int y = 0 ; y < n ; ++y){
                if(x == y) continue ;
                if(a[i]*b[x] > a[j]*b[y]) count++ ;
            }
        }
    }
}
ans = (ans*count)%mod ;
put(ans) ;

a[i]/a[j] > b'[y]/b'[x]
create a vector of pairs(x,y) and sort it in ascending order for pairs p1 and p2 if b[p1.first]*b[p2.second] > b[p2.first]*b[p1.second]
now for a given pair i and j binary search for that pair in this sorted vector and add the count
*/
vector<int> a,b ; 
struct custom{
    int y , x;
    custom(int y,int x):y(y),x(x){}
    bool operator<(const custom &other) const {
        return (b[y]*b[other.x]) < (b[other.y]*b[x]) ;
    }
}; 
void solve(){
    int n ;cin>>n ; 
    a.resize(n) ; b.resize(n) ;
    read(a) ;read(b) ; 
    if(n==1){
        put(0) ;
        return ;
    }
    int ans = exp(n*(n-1),mod-2,mod) ; // 1/(n*(n-1)) mod m 
    int count = 0 ;
    vector<custom> r ;
    for(int i = 0 ; i < n ; ++i){
        for(int j = 0 ; j < n ; ++j){
            if(i == j) continue ;
            r.emplace_back(i,j) ;
        }
    }
    sort(all(r)) ;
    for(int i = 0 ; i < n ; ++i){
        for(int j = i+1 ; j < n ; ++j){
            int low = 0 , high = (int)r.size() - 1 ;
            
            int pos = -1 ;
            while(low <= high){
                int mid = low + (high - low)/2 ;
                // a[i]/a[j] > b'[y]/b'[x]
                
                if(a[i]*b[r[mid].x] > a[j]*b[r[mid].y]){
                    pos = mid ;
                    low = mid+1 ;
                }
                else{
                    high = mid - 1 ;
                }
            }
            count += (pos == -1) ? 0 : pos + 1 ;
        }
    }
    count %= mod ;
    ans = (ans*count)%mod ;
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