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

*/
string to_bin(int x){
    if(x == 0) return "0";
    string res = "";
    while(x){
        res.push_back((x & 1) + '0');
        x >>= 1;
    }
    reverse(res.begin(), res.end());
    return res;
}
void solve(int s, int m){
    cout<<"sum: "<<s<<" "<<"mask: "<<m<<"\n" ; 
    vector<int> sum(s+1,INF)  ;  
    vector<int> used(s+1,-1);
    sum[0] = 0 ;   
    vector<int> par(s+1,-1) ;   
    for(int j = 0 ; j<=s ; ++j){
    	if(sum[j]==INF)continue ; 
    	for(int i = m;i>0 ; i= (i-1)&m){
    		if(j+i>s)continue ;  
    		if(sum[j]+1<sum[j+i]){
    			sum[j+i] = 1 + sum[j] ;  
    			par[j+i] = j ;  
    			used[j+i] = i;
    		}
    	}
    }
    if(sum[s]!=INF){
	    cout<<"MIN STEPS: "<<sum[s]<<"\n";
	    cout<<"Binary(STEPS): "<<to_bin(sum[s])<<"\n";

	    vector<int> moves;
	    int cur = s;

	    while(cur != 0 && used[cur] != -1){
		    moves.pb(used[cur]);
		    cur = par[cur];
		}

	    reverse(all(moves));

	    cout<<"Moves (decimal): ";
	    for(auto x : moves) cout<<x<<" ";
	    cout<<"\n";

	    cout<<"Moves (binary): ";
	    for(auto x : moves) cout<<to_bin(x)<<" ";
	    cout<<"\n";
	}
	else{
	    cout<<"-1\n";
	}	
	cout<<"\n\n" ; 
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
    

    while(tc--){
    	int s ; cin >> s ;  
    	for(int i = 1 ; i<=s ; ++i){
    		solve(s,i);
    	}
    }
    auto end = high_resolution_clock::now();
    duration<double> diff = end - start;
    cerr << fixed << setprecision(9) << diff.count() << "\n";
    return 0;
}



