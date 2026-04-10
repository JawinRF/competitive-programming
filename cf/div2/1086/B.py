import sys
input = lambda: sys.stdin.readline().rstrip()
t = int(input())

def f():
	n,k,p,m = map(int,input().split())
	p -= 1
	k -= 1
	a = list(map(int,input().split()))
	if k==n-1 : 
		print(m//a[p])
		return
	count = 0
	if p<=k:
		count += 1
		m -= a[p]
		value = a[p]
		a.pop(p)
		a.append(value)
		p = n-1
		
	while m>=a[p]:
		if p <= k : 
			count += 1
			m -= a[p]
			value = a[p]
			a.pop(p)
			a.append(value)
			p = n-1
			continue
		mn = 10**8
		idx = -1
		for i in range(k+1):
			if a[i]<mn:
				idx = i 
				mn = a[i]
		value = a[idx]
		a.pop(idx)
		a.append(value)
		m -= value
		
		p -= 1
	print(count)		
		
			
for _ in range(t):
	f()
