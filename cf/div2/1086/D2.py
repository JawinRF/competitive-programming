import sys
input = lambda: sys.stdin.readline().rstrip()
t = int(input())
from collections import deque

class DSU:
    def __init__(self,n):
        self.parent = list(range(n))
        self.size = [1]*n
        self.components = n 

    def find(self,i):
        if self.parent[i]==i:
            return i
        self.parent[i] = self.find(self.parent[i]) 
        return self.parent[i]

    def union(self,i,j):
        root_i = self.find(i)
        root_j = self.find(j)
        if root_i!=root_j:
            if self.size[root_i]<self.size[root_j]:
                root_i,root_j = root_j,root_i
            self.parent[root_j] = root_i
            self.size[root_i] += self.size[root_j]
            self.components -= 1
            return True
        return False

def f():
	n = int(input())
	m = [[0 for _ in range(n)] for _ in range(n)]
	adj = [[] for _ in range(n)]
	diag = 0
	for i in range(n):
		row = input().strip()
		for j in range(n):
			if row[j] == '1':
				if j!=i:m[i][j] = 1
				else: diag+=1
	if diag!=n:
		print("No")
		return	
	edge_count = 0	
	'''
	for i in range(n):
		for j in range(n):
			if not m[i][j]:continue
			flag = 0
			for k in range(n):
				if m[i][k] and m[k][j]:
					flag = 1
					break
			if not flag:
				tree[i][j] = 1
				edge_count += 1
	''' 
	sz = [ [m[i].count(1),i] for i in range(n) ]
	sz.sort(reverse = True)
	
	for i in range(n):
		size,u = sz[i]
		masked = [0]*n 
		for j in range(i+1,n):
			s2,v = sz[j] 
			if m[u][v]==1 and not masked[v]:
				adj[u].append(v)
				edge_count += 1
				if edge_count>=n:
					print("No")
					return
				for w in range(n):
					if m[v][w]==1:
						masked[w]=1
	if edge_count!=n-1 or diag!=n:
		print("No")
		return
	dsu = DSU(n)
	for i in range(n):
		for j in adj[i]:
			dsu.union(i,j)
	if dsu.components!=1:
		print("No")
		return
	for i in range(n):
        	visited = [0]*n
        	visited[i] = 1
        	q = deque([i])
        	while q:
        		curr = q.popleft()
        		for j in adj[curr]:
        			if not visited[j]:
        				visited[j] = 1
        				q.append(j)
        	for j in range(n):
        		if i != j:
        			if visited[j] != m[i][j]:
        				print("No")
        				return
	print("Yes")
	for i in range(n):
		for j in adj[i]:
			print(f"{i+1} {j+1}")
	
				
			
for _ in range(t):
	f()
