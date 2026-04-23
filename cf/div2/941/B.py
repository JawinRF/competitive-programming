import sys
input = lambda: sys.stdin.readline().rstrip()
t = int(input())

def f():
	n , m = map(int,input().split())
	a = [input() for _ in range(n)]
		
	mx_x , mn_x , mx_y ,mn_y = -1,n,-1,m
	for i in range(n):
		for j in range(m):
			if a[i][j]=='w':
				mx_x = max(i,mx_x)
				mn_x = min(i,mn_x)
				mx_y = max(j,mx_y)
				mn_y = min(j,mn_y)
	if (mn_x == 0 and mn_y == 0 and mx_x == n-1 and mx_y == m-1):
		print("YES")
		return
	mx_x , mn_x , mx_y ,mn_y = -1,n,-1,m
	for i in range(n):
		for j in range(m):
			if a[i][j]=='b':
				mx_x = max(i,mx_x)
				mn_x = min(i,mn_x)
				mx_y = max(j,mx_y)
				mn_y = min(j,mn_y)
	if (mn_x == 0 and mn_y == 0 and mx_x == n-1 and mx_y == m-1):
		print("YES")
		return
	print("NO")
for _ in range(t):
	f()
