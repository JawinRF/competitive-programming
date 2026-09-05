int dp[30];
class Solution {
public:
    Solution(){
        // from left to right at kth position from right
        // if bit = 1 then no of valid strings in k-2 bits
        // if bit = 0 then no of valid strings in k-1 bits
        // so dp[k]  = dp[k-2] + dp[k-1] ;
        // dp[0] = 1 , dp[1] = 2
        dp[0] = 1 ; dp[1] = 2 ;
        for(int i = 2 ; i<30 ; ++i ){
            dp[i] = dp[i-1] + dp[i-2] ;
        }
    }
    int findIntegers(int n) {
        // for a given if curr bit = 1 ans += dp[k-1]
        // because we can set curr bit = 0 then the curent number will be smaller than n
        // but then break if consecutive 1 bcz then n itself was invalid
        // if curr = 0 ntg
        int ans =  0 , prev = 0 ;
        for(int i = 30 ; i>=0 ; --i){
            if(n&(1<<i)){
                ans += dp[i] ;
                if(prev==1){
                    return ans ;
                }
                prev = 1 ;
            }
            else prev = 0 ;
        }
        return ans + 1;
    }
};
