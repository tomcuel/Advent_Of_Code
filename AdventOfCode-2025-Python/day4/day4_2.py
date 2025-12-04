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
is_available = True
while is_available:
    grid_copy = deepcopy(grid)
    count_taken_sup_0 = False 
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
                    count_taken_sup_0 = True
                    grid_copy[i][j] = 'x'

    grid = grid_copy
    if count_taken_sup_0 == False:
        is_available = False

print(total)
# for line in grid_copy:
#     print(''.join(line))
# found 43 as expected for the test.txt