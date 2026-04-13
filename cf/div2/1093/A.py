import sys
input = lambda: sys.stdin.readline().rstrip()
t = int(input())
from collections import deque

def f():
	n = int(input())
	a = list(map(int,input().split()))
	a.sort(reverse=True)
	s = set(a)
	if len(s)!=len(a):
		print(-1)
	else:
		for i in a:
			print(i,end=" ")
		print()

for _ in range(t):
	f()
