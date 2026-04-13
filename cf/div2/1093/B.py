import sys
input = lambda: sys.stdin.readline().rstrip()
t = int(input())
from collections import deque

def f():
	n , m = map(int,input().split())
	a = list(map(int,input().split()))
	l , mx = 0 ,0
	prev = -1
	for i in range(n):
		if a[i]==prev:
			l += 1
			mx = max(mx,l)
		else:
			l = 1 
		prev = a[i]
	if mx>=m:
		print("NO")
	else:
		print("YES")

for _ in range(t):
	f()
