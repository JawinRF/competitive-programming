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
MAXIMISE THE SUM WE WOULD LIKE TO HAVE 1 INCLUDED IN 
LEAST OF THE INTERVALS . LET 'I' BE THE 0 BASED POSITION OF 1
THEN NUMBER OF INTERVALS COVERING 1 are (I+1)*(N-I) .
To minimise the cnt we achieve it at the ends 
i.e at i = 0 or i = N-1 . 
once placed the rest forms a contiguous block of numbers in [2,n]
i.e the same subproblem for n-1 .
No of perm(n) = 2*perm(n-1) 
              = 2*2*perm(n-2)
              = 2^(n-1)
we can model this as a binary tree . 
where depth of a node describes how many nodes are placed 
like if depth is 2 then 1 ,2 are placed .
each edge is a placement operation  . 
if each left edge denotes placing on left end and edge right denotes placing on the right .
this will be a complete binary tree . 
Then we need to perfom a inorder traversal to find the kth permutation by checking leaf. 
but notice a left , left ,left ,... will land us on the 0th leaf
and if last left is replaced by right we land on the 1st leaf and so on .
essentially the binary representation of k will give us the path to the leaf .
6 so 6-1=5  = 101 which is right,left,right = [2,4,3,1]

here it is min() so max element has no control we spoke interms of the min element

*/
void solve(){
    int n , k ; cin >> n >> k ;
    // check k<=2^(n-1)
    // log(k) <= n-1
    // or ceil(log2(k))<=n-1
    if(ceil(log2(k)) > n-1){
        put(-1) ;
        return ;
    }
    k-- ; 
    
    vector<int> left , right ;
    int curr = 1 ; 
    for(int i = n-2 ; i>= 0 ; --i){
        //check if the bit is set or not
        // if(k&(1LL<<i)) right.pb(curr) ;
        // else left.pb(curr) ;
        // to prevent overflow we can do
        if((ceil(log2(k+1)))>i && (k&(1LL<<i))) right.pb(curr) ;
        else left.pb(curr) ;
        // cz higher than msb of k all are 0's so we can skip checking those bits
        // and put them in left
        curr++ ;
    }
    //place max element 
    right.pb(n) ;
    reverse(all(right)) ;  
    left.insert(left.end(),all(right)) ;
    for(int i = 0 ; i<n ; ++i){
        sput(left[i]) ;
    }
    cout<<"\n" ;
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