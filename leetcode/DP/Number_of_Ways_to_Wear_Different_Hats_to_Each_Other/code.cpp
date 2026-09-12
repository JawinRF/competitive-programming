class Solution {
public:
    const int MOD = 1e9+7 ; 
    int m ; 
    vector<vector<int>> H_To_P ; 
    vector<vector<int>> memo;

    int dfs(int currHat , int peopleMask){
        if(currHat==40){
            return peopleMask==((1LL<<m)-1) ; 
        }
        int& result = memo[currHat][peopleMask];
        if (result != -1) {
            return result;
        }
        int ways = 0 ;
        for(int people:H_To_P[currHat]){
            if(peopleMask & (1LL<<people)){
                continue ; 
            }
            int newMask = peopleMask | (1LL<<people) ; 
            ways = (ways + dfs(currHat+1,newMask))%MOD ; 
        }
        ways = ( ways + dfs(currHat+1,peopleMask))%MOD ; 
        return result=ways ; 
    }
    int numberWays(vector<vector<int>>& hats) {
        m = hats.size() ;  
        H_To_P.assign(40,vector<int> {}) ;
        for(int i = 0 ; i<m ; ++i){
            for(int hat:hats[i]){
                H_To_P[hat-1].push_back(i) ; 
            }
        }
        memo.assign(40, vector<int>(1 << m, -1));

        return dfs(0,0) ; 
    }
};
