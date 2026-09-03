#include <bits/stdc++.h>
using namespace std;

class DSU{
    vector<int> par , sz ;
public:
    DSU(int n){
        par.resize(n) ;
        sz.resize(n) ;
        for(int i = 0 ; i < n ; ++i){
            par[i] = i ;
            sz[i] = 1 ;
        }
    }
    int find(int a){
        if(par[a] == a) return a ;
        return par[a] = find(par[a]) ;
    }
    bool unite(int a , int b){
        a = find(a) , b = find(b) ;
        if(a == b) return false ;
        if(sz[a] < sz[b]) swap(a,b) ;
        par[b] = a ;
        sz[a] += sz[b] ;
        return true ;
    }
};
