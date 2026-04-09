import sys
input = lambda: sys.stdin.readline().rstrip()

t = int(input())

def s(a,n):
    	prf = [0]*n 
    	prf[0] = a[0]
    	for i in range(1,n):
    		prf[i] = max(prf[i-1],a[i])
    	c = 0
    	i = n-1
    	while i>=0:
    		while i>=0 and a[i]!=prf[i]:
    			i -= 1
    		c += 1
    		i -= 1	
    	print(c)
    
for _ in range(t):
	n = int(input())
	a = list(map(int,input().split()))
	s(a,n)
