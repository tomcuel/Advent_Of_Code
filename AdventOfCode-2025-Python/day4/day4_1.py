with open('input.txt', 'r') as f : 
    lines = f.read().strip().split('\n')
grid = [list(line) for line in lines]
n = len(grid)
m = len(grid[0])
# for line in grid:
#     print(''.join(line))

directions = [
    (1, 1), (1, 0), (1, -1),
    (0, -1),
    (-1, -1), (-1, 0), (-1, 1),
    (0, 1)
]

total = 0
from copy import deepcopy
grid_copy = deepcopy(grid)
for i in range(n):
    for j in range(m):
        if grid[i][j] == '@':
            count = 0
            for di, dj in directions:
                ni, nj = i + di, j + dj
                if 0 <= ni < n and 0 <= nj < m:
                    if grid[ni][nj] == '@':
                        count += 1

            if count < 4:  # accessible roll: fewer than 4 adjacent '@'
                total += 1
                grid_copy[i][j] = 'x'


print(total)
# for line in grid_copy:
#     print(''.join(line))
# found 13 as expected for the test.txt