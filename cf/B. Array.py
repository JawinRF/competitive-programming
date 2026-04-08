import sys
input = lambda: sys.stdin.readline().rstrip()

t = int(input())

def s(a,n):	
    
    ans = []  
    for i in range(n-1,-1,-1):
    	s , l  = 0,0
    	for j in range(i+1,n):
    		if a[j]>a[i]:
    			l += 1 
    		elif a[j]<a[i]:
    			s += 1
    	ans.append(max(s,l))
    
    ans.reverse()
    print(" ".join(map(str, ans)))
    		
    
for _ in range(t):
    n = int(input())
    a = list(map(int,input().split()))  
    s(a,n)
    

