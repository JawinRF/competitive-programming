import sys
input = lambda: sys.stdin.readline().rstrip()

t = int(input())

def s(a,n):
    	idx = a.find('L')
    	if idx==-1:
    		print(n)
    	else:
    		print(idx+1)
    
for _ in range(t):
	n = int(input())
	a = input()
	s(a,n)
