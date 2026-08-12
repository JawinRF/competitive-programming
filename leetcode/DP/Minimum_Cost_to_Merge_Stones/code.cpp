#include <bits/stdc++.h>
using namespace std;

/*
LeetCode 1000 - Minimum Cost to Merge Stones (Hard)

STATEMENT
There are n piles of stones in a row. The i-th pile has stones[i] stones.
A move merges exactly k consecutive piles into one pile. The cost of the move
is the total number of stones in those k piles.
Return the minimum total cost to merge all piles into one pile. If that is not
possible, return -1.

Constraints:
    n == stones.length
    1 <= n <= 30
    1 <= stones[i] <= 100
    2 <= k <= 30

Examples:
    stones = [3,2,4,1], k = 2  ->  20
        [3,2] -> 5 costs 5, giving [5,4,1]
        [4,1] -> 5 costs 5, giving [5,5]
        [5,5] -> 10 costs 10, giving [10].  Total 20.
    stones = [3,2,4,1], k = 3  ->  -1
        Any move leaves 2 piles, and 2 piles cannot make a move of 3.
    stones = [3,5,1,2,6], k = 3  ->  25
        [5,1,2] -> 8 costs 8, giving [3,8,6]
        [3,8,6] -> 17 costs 17, giving [17].  Total 25.

SOLUTION
Feasible only if (n-1) % (k-1) == 0, because every move drops the pile count
by k-1. The code tests (n-k) % (k-1), which is the same residue.

Interval DP with a pile-count dimension:
    dp[i][j][t] = min cost to turn stones[i..j] into t piles
    dp[i][j][t] = min over split m and left count a of
                      dp[i][m][a] + dp[m+1][j][t-a]        (a split pays nothing)
    dp[i][j][1] = dp[i][j][k] + sum(i..j)                  (the merge pays)
Base: dp[i][i+d-1][d] = 0, since d elements already are d piles.
Order: for a fixed (i,j) do every t >= 2 first, then t == 1.
Answer: dp[0][n-1][1].

The split loop tries every left count a. Fixing a = 1 also reaches every
optimum and drops a factor of k.
*/

const int N = 30 ;
int dp[N][N][N+1] ;
const int INF = (1LL<<29) ;
class Solution {
public:
    int mergeStones(vector<int>& stones, int k) {
        int n = stones.size() ;
        if ( (n-k)%(k-1) != 0){
            return -1 ;
        }
        for(int i = 0 ; i<n ; ++i){
            for(int j = 0 ; j<n ; ++j){
                for(int m = 0 ; m<=k ; ++m){
                    dp[i][j][m] = INF ;
                }
            }
        }
        // base case
        // initialize
        // dp[i][i+k][k] = 0 because we already have k piles

        // infact dp[i][i+delta-1][delta] where delta>0 and delta<=k is 0
        vector<int> pref = stones ;
        for(int i = 1 ; i<n ; ++i)pref[i] += pref[i-1] ;

        for(int i = 0 ; i<n ; ++i){
            for(int delta=1 ; delta<=k ; ++delta){
                if(i+delta-1<n){
                    dp[i][i+delta-1][delta] = 0 ;
                }
            }
        }

        // state transition
        // ( i , j , m )
        // can be computed as

        // ( i , k , a ) + ( k+1,j , m-a)

        // but only for dp[i][j][1] = dp[i][j][k] + sum(i...j ) after all (i,j,*) transition

        for(int interval_length = 1 ; interval_length<=n ; ++interval_length){
            for(int i = 0 ; i<n ; ++i){
                int j = i+interval_length-1 ;
                if(j>=n)break ; // early exit
                //enumerate every 3rd dimension for i,j
                for(int t = 2 ; t<=k ; ++t){
                    //loop through splits
                    for(int m = i ; m<j ; ++m){
                        // try all left pile possibilites and right
                        for(int delta=1 ; delta<t ; ++delta){

                            if(dp[i][m][delta]!=INF && dp[m+1][j][t-delta]!=INF){

                                dp[i][j][t] = min( dp[i][j][t] , dp[i][m][delta] + dp[m+1][j][t-delta] ) ;
                            }
                        }
                    }
                }
                if((interval_length - 1)%(k-1)==0  && (dp[i][j][k] != INF)){
                    dp[i][j][1] = min( dp[i][j][1] ,
                        dp[i][j][k] + pref[j] - (i > 0 ? pref[i-1] : 0)
                    );
                }
            }
        }
        return dp[0][n-1][1] ;
    }
};
