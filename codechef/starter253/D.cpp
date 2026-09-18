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
|pos[i] - i| >= |pos[j] - j|
pos[i] = current position of value i.

we need to show |i - j| >= |pos[i] - pos[j]|
Choose i with maximum |pos[i] - i|.
let 
j = pos[i].
Then 

|pos[i] - i| >= |pos[j] - j|
becomes
|j - i| >= |pos[j] - pos[i]|

or equivalently

|i - j| >= |pos[i] - pos[j]|

which is exactly the condition required to swap the elements
at positions pos[i] and pos[j].

So we can swap:

P[pos[i]] and P[pos[j]]

Since P[pos[i]] = i
and   P[pos[j]] = j,

this swaps values i and j.

After the swap, value j goes to position j, so j becomes fixed.

*/
void solve(){
    int n;
    cin >> n;

    vector<int> a(n + 1), pos(n + 1);

    for(int i = 1; i <= n; ++i){
        cin >> a[i];
        pos[a[i]] = i;
    }

    priority_queue<pair<int,int>> pq;

    for(int i = 1; i <= n; ++i){
        pq.push({abs(pos[i] - i), i});
    }

    vector<pair<int,int>> ans;

    while(!pq.empty()){
        auto [diff, i] = pq.top();
        pq.pop();

        // stale entry
        if(abs(pos[i] - i) != diff) continue;

        // everything is fixed
        if(diff == 0) break;

        int j = pos[i];

        int pi = pos[i];
        int pj = pos[j];

        //swap positions pos[i], pos[j]
        ans.emplace_back(pi, pj);

        swap(a[pi], a[pj]);

        //values i and j exchanged their positions
        swap(pos[i], pos[j]);

        // j is now fixed but i may still be misplaced.
        pq.push({abs(pos[i] - i), i});
    }

    cout << ans.size() << "\n";

    for(auto &[x, y] : ans){
        cout << x << " " << y << "\n";
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