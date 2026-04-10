import sys
input = lambda: sys.stdin.readline().rstrip()
t = int(input())

def f():
	n = int(input())
	d = dict()
	mx = 0
	for i in range(n):
		for x in map(int, input().split()):
			if x in d:
				d[x] += 1
			else:
				d[x] = 1
			mx = max(mx, d[x])
	if mx <= n*n - n:
		print("YES")
	else:
		print("NO")
			
for _ in range(t):
	f()
