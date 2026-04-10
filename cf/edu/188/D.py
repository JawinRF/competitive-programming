import sys
input = lambda: sys.stdin.readline().rstrip()
sys.setrecursionlimit(300000)
t = int(input())

def s():
	n , m  = map(int,input().split())
	l = [ [] for _ in range(n) ] 
	for j in range(m):
		a,b = map(int,input().split())
		a -= 1 
		b -= 1
		l[a].append(b)
		l[b].append(a)
	c = [-1]*n
	dpt = [0]*n
	bad = set()
	tot,s1 = [0]*n,[0]*n
	num = 0
	'''
	def dfs( node , d ,par ,color):
		if c[node]!=-1:
			if (d - dpt[node])%2==1:
				bad.add(num)
			return
		c[node] = num
		dpt[node] = d
		tot[num] += 1
		if color==1:
			s1[num] += 1
		for x in l[node]:
			if x == par:continue
			dfs(x,d+1,node,color^1)
	'''
	def dfs( node , d ,par ,color):
		stk = [(node,d,par,color)]
		while stk:
			node,d,par,color = stk.pop()
			if c[node]!=-1:
				if (d-dpt[node])%2==1:
					bad.add(num)
				continue
			c[node] = num
			dpt[node] = d
			tot[num] += 1
			if color == 1:
				s1[num] += 1
			for x in l[node]:
				if x==par:continue
				stk.append((x,d+1,node,color^1))
	ans = 0
	for i in range(n):
		if c[i]==-1:
			dfs(i,0,-1,0)
			if num not in bad:
				ans += max(s1[num],tot[num]-s1[num])
			num += 1
	print(ans)
	
for _ in range(t):
	s()
    
