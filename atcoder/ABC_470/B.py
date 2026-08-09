# ABC 470 B - N balls, ball i has color C_i (1..N). One operation recolors a
# single ball to any color. Minimum operations to make all balls one color:
# keep the most frequent color, repaint the rest -> n - max frequency.

n = int(input())
l = list(map(int, input().split()))

d = dict()
for a in l:
    if a not in d:
        d[a] = 1
    else:
        d[a] = d[a] + 1

mx = 0
for k, v in d.items():
    mx = max(v, mx)

print(n - mx)
