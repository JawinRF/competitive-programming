import sys
input = lambda: sys.stdin.readline().rstrip()

t = int(input())

def s(r, g, b):
    
    n = r + g + b
    
    maxlen = 1
    dp = [[[[[0, 0, 0] for _ in range(4)] for _ in range(4)] for _ in range(4)] for _ in range(n + 1)]
    parent = [[[[None for _ in range(4)] for _ in range(4)] for _ in range(4)] for _ in range(n + 1)]
    for x in range(3):
        if x == 0 and r >= 1:
            dp[1][x][3][3] = [1, 0, 0]
        elif x == 1 and g >= 1:
            dp[1][x][3][3] = [0, 1, 0]
        elif x == 2 and b >= 1:
            dp[1][x][3][3] = [0, 0, 1]
    
    
    best_state = None
    for i in range(1, n):
        for x in range(4):
            for y in range(4):
                for z in range(4):
                    if dp[i][x][y][z] == [0, 0, 0]:
                        continue
                    
                    for nxt in range(3):
                        R, G, B = dp[i][x][y][z]
                        
                        if nxt == 0:
                            R += 1
                        elif nxt == 1:
                            G += 1
                        else:
                            B += 1

                        if R <= r and G <= g and B <= b and nxt != z and nxt != x:
                            dp[i + 1][nxt][x][y] = [R, G, B]
                            parent[i + 1][nxt][x][y] = (x, y, z)
                            maxlen = i + 1
                            best_state = (i+1,nxt,x,y)
    if maxlen==1:
    	if r>=1:print("R")
    	elif g>=1:print("G")
    	elif b>=1:print("B")
    	return
    length,c_x,c_y,c_z = best_state
    ans = []
    mpp = {0:'R',1:'G',2:'B'}
    
    while length > 0:
        ans.append(mpp[c_x])
        prev = parent[length][c_x][c_y][c_z]
        if prev is None: 
            break
        c_x, c_y, c_z = prev
        length -= 1
     
    print("".join(ans[::-1]))  
    
for _ in range(t):
    r, g, b = map(int, input().split())
    s(r, g, b)
