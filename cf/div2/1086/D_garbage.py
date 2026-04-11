import sys
input = lambda: sys.stdin.readline().rstrip()
t = int(input())
from collections import deque
import copy

def f():
	n = int(input())
	m = [[0 for _ in range(n)] for _ in range(n)]
	
	tree = copy.deepcopy(m)
	indegree = [0]*n
	for i in range(n):
		row = input().strip()
		for j in range(n):
			if row[j] == '1' and j!=i:
				m[i][j] = 1
				indegree[j] += 1
	d = deque()
	for i in range(n):
		if indegree[i]==0:
			d.append(i)
	seq = []
	while d:
		node = d.popleft()
		seq.append(node)
		for i in range(n):
			if m[node][i]:
				indegree[i] -= 1
				if indegree[i] == 0:
					tree[node][i] = 1
					d.append(i)
	if indegree.count(0)!=n:
		print("No")
		return
	print("Yes")
	for i in range(n):
		for j in range(n):
			if tree[i][j]:
				print(f"{i+1} {j+1}")
	
				
			
for _ in range(t):
	f()
