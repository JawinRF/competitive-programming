#include <bits/stdc++.h>
#pragma GCC optimize("O3", "unroll-loops")
#define ll long long
#define eb emplace_back
using namespace std;
int mod = 1e9+7;

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
    int memo[20][77][39][39];
    int k2, k3, k5;
    int n;
    vector<vector<int>> facts;

    int dfs(int idx, int p2, int p3, int p5) {
        if (idx == n) {
            return (p2 == k2 && p3 == k3 && p5 == k5) ? 1 : 0;
        }

        int &res = memo[idx][p2 + 38][p3 + 19][p5 + 19];
        if (res != -1) return res;

        int ans = 0;
        int f2 = facts[idx][0];
        int f3 = facts[idx][1];
        int f5 = facts[idx][2];

        ans += dfs(idx + 1, p2 + f2, p3 + f3, p5 + f5);
        ans += dfs(idx + 1, p2 - f2, p3 - f3, p5 - f5);
        ans += dfs(idx + 1, p2, p3, p5);

        return res = ans;
    }
    int countSequences(vector<int>& nums, ll k) {
    	// THIS IS A DP SOLUTION BUT CAN ALSO BE SOLVED USING MEET IN MIDDLE
        ll temp = k;
        k2 = k3 = k5 = 0;
        
        while (temp % 2 == 0) { k2++; temp /= 2; }
        while (temp % 3 == 0) { k3++; temp /= 3; }
        while (temp % 5 == 0) { k5++; temp /= 5; }
        if(temp>1)return 0;
        n = nums.size();
        facts.assign(n, vector<int>(3, 0));

        for (int i = 0; i < n; i++) {
            int x = nums[i];
            while (x % 2 == 0) { facts[i][0]++; x /= 2; }
            while (x % 3 == 0) { facts[i][1]++; x /= 3; }
            while (x % 5 == 0) { facts[i][2]++; x /= 5; }
        }

        memset(memo, -1, sizeof(memo));

        return dfs(0, 0, 0, 0);
    }
};


