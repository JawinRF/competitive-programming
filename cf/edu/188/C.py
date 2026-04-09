import sys
from math import gcd
input = lambda: sys.stdin.readline().rstrip()

t = int(input())
def lcm(a,b):
	return a*b//gcd(a,b)
def s(a,b,c,m):
	# for a
	p = (m//a)*6 - (m//lcm(b,a))*3 - (m//lcm(a,c))*3 + (m//lcm(a,lcm(b,c)))*2
	print(p,end=" ")
    
for _ in range(t):
	a,b,c,m = map(int,input().split())
	s(a,b,c,m)
	s(b,c,a,m)
	s(c,a,b,m)
	print()
