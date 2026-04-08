from math import gcd
import sys
input = lambda: sys.stdin.readline().rstrip()
def lcm(x,y):
    return (x*y)//gcd(x, y)


t = int(input())
p = [1, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73]
m = len(p)

def s(a,b,n):
    # dp[i][j] be the most operations that can be done upholding the constraint
    c = [0]*n
    for i in range(0,n):
        g1 = gcd(a[i-1],a[i]) if i>0 else -1
        g2 = gcd(a[i],a[i+1]) if i<n-1 else -1
        if g1==-1:
            g1 = g2
        elif g2==-1:
            g2 = g1
        c[i] = lcm(g1,g2)
        if c[i]>b[i]:
            c[i] = a[i]
    
    dp = [[-10**18] * m for _ in range(n)]
    
    # base
    for j in range(m):
    	if j == 0 : 
    		if c[0]==a[0]:
    			dp[0][0] = 0 
    		else :dp[0][0] = 1 
    		continue
    	new_val = c[0]*p[j] 
    	if new_val<=b[0] and new_val!=a[0] and gcd(new_val,c[1])==gcd(a[0],a[1]) :
    		dp[0][j] = 1
    
    #transition
    for i in range(1,n):
    	for j in range(m):
    		for k in range(m):
    			
    			if j==0:
    				if c[i]==a[i]:dp[i][j] = max(dp[i][j],dp[i-1][k])
    				else :dp[i][j] = max(dp[i][j] , dp[i-1][k] + 1 )
    				continue
    			
    			new_curr = c[i]*p[j]
    			prev = c[i-1]*p[k]
    			
    			if new_curr<=b[i] and gcd(new_curr,prev)==gcd(a[i],a[i-1]) and new_curr!=a[i]:
    				if i==n-1:
    					dp[i][j] = max(dp[i][j],dp[i-1][k]+1)
    				elif gcd(new_curr,c[i+1])==gcd(a[i],a[i+1]):
    					dp[i][j] = max(dp[i][j],dp[i-1][k]+1)		
    print(max(0,max(dp[n-1])))		
    				
    
for _ in range(t):
    n = int(input())
    a = list(map(int,input().split()))  
    b = list(map(int,input().split()))  
    s(a,b,n) 
    
