class Solution {
public:
    int tallestBillboard(vector<int>& rods) {
        int S=accumulate(rods.begin(),rods.end(),0);
        vector<int> dp(2*S+1,-1);
        dp[S]=0;
        // we can achive difference zero by having 2 towers of 
        // zero height by default

        for(int x:rods){
            vector<int> ndp=dp;
            for(int d=-S;d<=S;d++){
                if(dp[d+S]==-1) continue;

                // put in left
                if(d+x<=S)
                    ndp[d+x+S]=max(ndp[d+x+S],dp[d+S]+x);
                //put in right
                if(d-x>=-S)
                    ndp[d-x+S]=max(ndp[d-x+S],dp[d+S]);
            }
            dp.swap(ndp);
        }
        return dp[S];
    }
};;
