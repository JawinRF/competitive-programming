import sys
input = lambda: sys.stdin.readline().rstrip()
t = int(input())

def f():
	n , k = map(int,input().split())
	a = list(map(int,input().split()))
	d = {}
	mx = 0
	for x in a:
		d[x] = d.get(x,0) + 1
		mx = max( mx , d[x] )
	if mx>=k:
		print(k-1)
	else:
		print(n)
		
for _ in range(t):
	f()
