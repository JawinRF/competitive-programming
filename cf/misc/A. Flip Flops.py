import sys
input = lambda: sys.stdin.readline().rstrip()

t = int(input())

def s(a,c,n,k):	
    a.sort() 
    for i in range(0,n):
    	if a[i]>c:
    		break
    	
    	flips = min(k,c-a[i])
    	c += a[i]+flips
    	k -= flips
    print(c)
    		
    
for _ in range(t):
    n,c,k = map(int,input().split())
    a = list(map(int,input().split()))  
    s(a,c,n,k) 
    

