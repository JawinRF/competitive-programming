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
c1 < c2
to get the smallest number larger than x

*/
bool larger(string x , int i , char c1 , char c2,string &ans,bool prefBigger){
    if(i==(int)x.size()){
        return true ;
    }
    if(prefBigger){
        fill(ans.begin() + i, ans.end(), c1);
        return true; 
    }
    for(char c : {c1 , c2}){
        if(c<x[i])continue ; 
        if(larger(x , i+1 , c1 , c2 , ans , c>x[i])){
            ans[i] = c ;
            return true ;
        }
    }
    return false ;
}
bool smaller(string x , int i , char c1 , char c2 , string &ans , bool prefSmaller){
    if(i == (int)x.size()){
        return true;
    }
    if(prefSmaller){
        fill(ans.begin() + i, ans.end(), c2);
        return true;
    }

    for(char c : {c2, c1}){
        if(c > x[i]) continue;

        if(smaller(x, i + 1, c1, c2, ans, c < x[i])){
            ans[i] = c;
            return true;
        }
    }

    return false;
}
void solve(){
    int a , n ;cin >> a >> n ; 
    char c1, c2 ; cin >> c1 >> c2 ;
    if(c1>c2){
        swap(c1,c2) ;
    }
    string x = to_string(a) ;
    n  = x.length() ; 
    string bigg(n,'0')  , small(n,'9') ;
    int mx = 2e18, mn = 0;
    if(larger(x , 0 , c1 , c2 , bigg , false)){
        mx = stoll(bigg);
    }
    else{
        if (c1 == '0') {
            bigg = string(1, c2) + string(n, '0');
        }
        else{
            bigg = string(n+1,c1);
        }
        mx = stoll(bigg);
    }
    bool valid = true ; //lower bound doesnt always exist but upper bound always exists
    if(smaller(x,0,c1,c2,small,false)){
        mn = stoll(small) ;
    }
    else if(n!=1){
        small = string(n-1,c2) ;
        mn = stoll(small) ;
    }else{
        valid = false ;
    }
    if(valid){
        put(min(mx-a,a-mn));
        return ;
    }
    put(mx-a);
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