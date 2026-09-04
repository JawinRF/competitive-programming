#include <bits/stdc++.h>
using namespace std;

class Solution { 
public:
    int n ;  
    unordered_map<int,int> best1,best2 ; 
    void f(int i , int left , int right ,vector<int> &rods, bool Call,int end){
        if (i==end){
            if(Call){
                best2[left-right] = max(best2[left-right],left) ;
            }
            else best1[left-right] = max(best1[left-right],left);  
            return ;
        }
        f(i+1,left+rods[i],right,rods,Call,end) ;  
        f(i+1,left,right+rods[i],rods,Call,end) ;  
        f(i+1,left,right,rods,Call,end) ;
    }
    int tallestBillboard(vector<int>& rods) {
        n = rods.size() ;
        best1.clear() ; best2.clear() ;  

        int m = n/2 ;
        f(0,0,0,rods,0,m) ;
        f(m,0,0,rods,1,n) ;

        int ans = 0 ;
        for(auto &[x,y]:best1){
            if(best2.count(-x)){
                ans = max(ans,y+best2[-x]) ;  
            }
        }
        return ans ;  
    }
};
