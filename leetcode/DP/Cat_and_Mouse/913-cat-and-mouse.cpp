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

/*

*/

const int N = 52; 
// deg[m][c][turn] be the count of states that are determined to be loosing
// for the player 'turn' and are reachable from (m,c,turn)

int deg[N][N][2] ;
int res[N][N][2] ;
class Solution {
public:
    int catMouseGame(vector<vector<int>>& graph) {
        int n = graph.size() ;  
        // mouse win : 1 
        // cat win : 2
        // draw : 0
        for(int i = 0 ; i<n ; ++i){
            for(int j = 0 ; j<n ; ++j){
                deg[i][j][0] = 0 ; 
                deg[i][j][1] = 0 ;
            }
        }
        // for (m,c,turn) 
        // if mouse turn i.e turn==0, cntLosing
        // for cat:
        // all neighbouring states must be mouse win to be cat LOOSE
        // any neighbouring state is cat win to be cat WIN
        // for mouse:
        // any neighbouring state is mouse win to be mouse WIN
        // all neighbouring states must be cat win to be mouse LOOSE
        for(int i = 0 ; i<n ; ++i){
            for(int j = 0 ; j<n ; ++j){
                res[i][j][0] = -1 ; 
                res[i][j][1] = -1; 
            }
        }
        queue<tuple<int,int,int>> q ;
        // Add base cases to the queue
        // base case
        // (m,c,turn)
        // (0,c,turn) = 1
        for(int c = 1 ; c<n ; ++c){
            res[0][c][0] = 1 ; 
            res[0][c][1] = 1 ; 
            q.push({0,c,1}) ;
            q.push({0,c,0}) ;
        }
        // (C,C,turn) = 2
        for(int c = 1 ; c<n ; ++c){
            res[c][c][0] = 2 ; 
            res[c][c][1] = 2 ; 
            q.push({c,c,1}) ;
            q.push({c,c,0}) ;
        }

        vector<int> catDeg(n) ;
        // since cat cannot move to 0, so deg[cat] = graph[cat].size()-1
        for(int c = 0 ; c<n ; ++c){
            catDeg[c] = graph[c].size() ;
            for(int neigh : graph[c]){
                if(neigh==0){
                    catDeg[c]-- ;
                    break ;
                }
            }
        }
        while(!q.empty()){
            auto [m,c,turn] = q.front() ;
            q.pop() ;
            int prevTurn = 1 - turn ;
            // if its mouse turn, then previous turn was cat turn
            // so (m,c',prevTurn) -> (m,c,turn)
            // else if its cat turn, then previous turn was mouse turn
            // so (m',c,prevTurn) -> (m,c,turn)
            int prevM , prevC ;
            if(turn==0){
                prevM = m ;
                // graph[c] cz move was from cat 
                for(int neigh : graph[c]){
                    prevC = neigh ;
                    if(prevC==0 || res[prevM][prevC][prevTurn]!=-1) continue ;
                    if(res[m][c][0] == 2){
                        //cat wins by performing
                        // (prevM,prevC,1)->(m,c,0)
                        res[prevM][prevC][1] = 2 ; 
                        q.push({prevM,prevC,1}) ;
                    }
                    else{
                        // (m,c,0) is mouse win
                        // so no of losing states reachable from (prevM,prevC,1) is increased
                        deg[prevM][prevC][1]++ ;
                        if(deg[prevM][prevC][1]==catDeg[prevC]){
                            res[prevM][prevC][1] = 1 ;
                            q.push({prevM,prevC,1}) ;
                        }
                    }
                }
            }
            else{
                prevC = c ;
                // graph[m] cz move was from mouse
                for(int neigh : graph[m]){
                    prevM = neigh ;
                    if(prevC==0 || res[prevM][prevC][prevTurn]!=-1) continue ;
                    if(res[m][c][1] == 2){
                        // (prevM,prevC,0)->(m,c,1)
                        // so mouses looses if it transitions from (prevM,prevC,0) to (m,c,1)
                        deg[prevM][prevC][0]++ ; 
                        if(deg[prevM][prevC][0]==graph[prevM].size()){
                            // Then (prevM,prevC,0) is a lost state for cat
                            res[prevM][prevC][0] = 2 ;
                            q.push({prevM,prevC,0}) ;
                        }
                    }
                    else{
                        // (m,c,1) is mouse win
                        // so mouse wins if it transitions from (prevM,prevC,0) to (m,c,1)
                        res[prevM][prevC][0] = 1 ;
                        q.push({prevM,prevC,0}) ;
                    }
                }
            }
        }
        for(int i = 0 ; i<n ; ++i){
            for(int j = 0 ; j<n ; ++j){
                if(res[i][j][0] == -1) res[i][j][0] = 0 ;  
                if(res[i][j][1] == -1) res[i][j][1] = 0 ;
            }
        }
        return res[1][2][0] ;
    }
};