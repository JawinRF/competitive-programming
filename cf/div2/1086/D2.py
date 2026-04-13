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

m = [bytearray(8000) for _ in range(8000)]
adj = [[] for _ in range(8000)]

def f():
	n = int(input())
	for i in range(n):
		adj[i].clear()
		for j in range(n):
			m[i][j] = 0

	diag = 0
	deg = [0]*n
	for i in range(n):
		row = input().strip()
		for j in range(n):
			if row[j] == '1':
				if j!=i:
					m[i][j] = 1
					deg[i] += 1
				else:
					diag+=1

	if diag!=n:
		print("No")
		return	

	edge_count = 0	

	sz = [[deg[i],i] for i in range(n)]
	sz.sort(reverse = True)
	
	for i in range(n):
		size,u = sz[i]
		masked = bytearray(n)
		for j in range(i+1,n):
			s2,v = sz[j] 
			if m[u][v]==1 and not masked[v]:
				adj[u].append(v)
				edge_count += 1
				if edge_count>=n:
					print("No")
					return
				row = m[v]
				for w in range(n):
					if row[w]==1:
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
		visited = bytearray(n)
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
		for v in adj[i]:
			print(f"{i+1} {v+1}")

for _ in range(t):
	f()
