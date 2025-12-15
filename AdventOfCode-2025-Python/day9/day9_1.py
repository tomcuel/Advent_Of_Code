with open('input.txt', 'r') as f:
    lines = f.read().strip().split('\n')

lines = [list(map(int, line.split(','))) for line in lines]
"""
n = max(y for x, y in lines) + 1
m = max(x for x, y in lines) + 1

grid = [['.' for _ in range(m)] for _ in range(n)]
coords = {(y, x) for x, y in lines}
for i in range(n):
    for j in range(m):
        if (i, j) in coords:
            grid[i][j] = '#'

for line in grid:
    print(''.join(line))
"""

# for each tile couple, calculate the rectangle area associated 
# x1, y1 and x2, y2 --> abs(x1-x2)+1 * abs(y1-y2)+1 
from itertools import combinations
areas = [] 
for (x1, y1), (x2, y2) in combinations(lines, 2):
    area = (abs(x1 - x2)+1) * (abs(y1 - y2)+1)
    areas.append((area, (x1, y1), (x2, y2)))

max_area, A, B = max(areas, key=lambda x: x[0])
print(max_area, A, B)
# found 50 as expected for the test.txt