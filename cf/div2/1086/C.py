import sys
input = lambda: sys.stdin.readline().rstrip()
t = int(input())


'''
S  
-> S*(100-P[i1])/100

->S*(100-P[i1])/100 * (100-P[i2])/100

in general 
after picking k items 

-> S/100^k * ( 100 - P[i1] )*( 100 - P[i2] )*( 100 - P[i3] ) ... 
let t[i] = 100 - P[i] and since S = 1 

-> 100^(-k) * t[i1] * t[i2] * .... 

Now points

 0
 -> S*c[i1]
 -> S*c[i1]  + S/100 *t[i1] * c[i2]

In general  
after picking k items

Stamina = 100^(-k) * t[i1] * t[i2] ... t[ik]

Points =  c[i1] + 100^(-1)*t[i1]*c[i2] + ... + 100^(-(k-1))*t[i1]*...t[i(k-2)]*c[ik]
	

Notice 
Stamina = 100^(-1) * t[i1] * ( stamina from picking k-1 items from the rest) 
Points =  c[i1] + 100^(-1)*t[i1]*( points from picking k-1 items from rest) 
So its essential to process from right and points is indenpendent of Stamina

'''
def f():
	n = int(input())
	c = []
	t = []
	for _ in range(n):
		ci, pi = map(int, input().split())
		c.append(ci)
		t.append(100 - pi)
	p = 0.0	
	for i in range(n-1,-1,-1):
		p = max(p,c[i] + (t[i]/100)*p)
	print(f"{p:.20f}")
	
			
for _ in range(t):
	f()
