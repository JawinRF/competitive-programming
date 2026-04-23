import sys
input = lambda: sys.stdin.readline().rstrip()
t = int(input())
from collections import deque

''' 
horizontal  = (n+1)*m 
ver =  (n)*(m+1)
tot = 2nm + m + n
p,q = > tot = p + 2*q

2nm + m + n = p + 2*q
4nm + 2m + 2n + 1 = 2(p+2q) + 1
(2m+1)*(2n+1) = 2(p+2q) + 1

q<=hori and q<=vert
'''

def f():
	p , q = map(int,input().split())
	t = 2*(p+2*q)+1
	for i in range(3,int((t + 1)**0.5)):
		if t%i==0:
			# 2n+1 =i
			n = (i-1)//2
			# 2m+1 = t/i 
			m = (t//i - 1)//2
			
			if q<=(n+1)*m and q<=(m+1)*n:
				print(f"{n} {m}")
				return
	print(-1)
		
for _ in range(t):
	f()
