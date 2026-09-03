#include <bits/stdc++.h>
#pragma GCC optimize("O3", "unroll-loops")
#define ll long long
#define eb emplace_back
using namespace std;
ll mod = 1e9+7;
ll int INF = 1e18;
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

class Solution {
public:
    string shortestSuperstring(vector<string>& words) {
        int n = words.size() ;
        int N = 1LL<<n ;
        vector<vector<int>> overlap(n,vector<int> (n,0))  ;

        for(int i = 0 ; i<n ; ++i){
            for(int j = 0 ; j<n ; ++j){
                for(int c = 0 ; c<words[i].size(); ++c){
                    bool ok = true ;
                    for(int k = c ; k<words[i].size() ; ++k){
                        if(k-c<words[j].size()){
                            ok &= words[i][k]==words[j][k-c] ;
                        }
                        else ok = false ;
                    }
                    if(ok){
                        overlap[i][j] = (int)words[i].size() - c ;
                        break ;
                    }
                }
            }
        }

        vector<vector<int>> dp(N,vector<int> (n,-1));
        vector<vector<int>> parent(N,vector<int> (n,-1));

        for(int i = 0 ; i<n ; ++i){
            dp[1<<i][i] = 0 ;
        }

        for(int i = 0  ; i<N ; ++i){
            for(int j = 0 ; j<n ; ++j){
                if(dp[i][j]!=-1){
                    for(int k =  0; k<n ; ++k){
                        if(!(i&(1LL<<k))){
                            int newMask = i | (1LL<<k) ;
                            int newValue = overlap[j][k] + dp[i][j] ;

                            if(newValue > dp[newMask][k]){
                                dp[newMask][k] = newValue ;
                                parent[newMask][k] = j ;
                            }
                        }
                    }
                }
            }
        }

        int last = max_element(all(dp[N-1])) - dp[N-1].begin() ;
        vector<int> order ;
        int mask = N-1 ;

        while(last!=-1){
            order.pb(last) ;

            int p = parent[mask][last] ;
            mask ^= (1LL<<last) ;
            last = p ;
        }

        reverse(all(order)) ;

        string ans = words[order[0]] ;

        for(int i = 1 ; i<order.size() ; ++i){
            int prev = order[i-1] ;
            int cur = order[i] ;
            ans += words[cur].substr(overlap[prev][cur]) ;
        }

        return ans ;
    }
};
