import sys


t = int(input())


'''
 consider 1,2,3
 check (1,2) , (1,3) , (2,3)  
 if all return res = 0 
 then there is one block among (4,5) , (6,7) , ... ,(2n-2,2n-1) has only zeros 
 
 we have spent 3
 n-2 are left 
 = n+1 queries

'''
def s(n):
    print("? 1 2",flush=True)
    res = int(input())
    if res == 1:
    	print("! 1",flush=True)
    	return
    	
    print("? 1 3",flush=True)
    res = int(input())
    if res == 1:
    	print("! 1",flush=True)
    	return
    	
    print("? 2 3",flush=True)
    res = int(input())
    if res == 1:
    	print("! 2",flush=True)
    	return
    
    for i in range(4,2*n,2):
    	print(f"? {i} {i+1}",flush=True)
    	res = int(input())
    	if res == 1:
    		print(f"! {i}",flush=True)
    		return
    print(f"! {2*n}",flush=True)
for _ in range(t):
    n = int(input())
    s(n)
    

