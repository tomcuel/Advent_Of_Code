from heapq import heappush, heappop
from collections import defaultdict

# Opening the file
with open('input18.txt', 'r') as file:
    memory_space = file.read().strip().split('\n')
memory_space = [[int (i) for i in line.split(',')] for line in memory_space]
map_size = 71 # 71x71 grid for the input, 7x7 for the test
map = [['.' for i in range(map_size)] for j in range(map_size)]
def is_within_bounds(x, y):
    return x >= 0 and x < map_size and y >= 0 and y < map_size
directions = [[0,1], [0,-1], [1,0], [-1,0]]

# part1 is placing the memory_space in the map
number_falls = 1024 # 1024 for the input, 12 for the test
for i in range(number_falls):
    map[memory_space[i][0]][memory_space[i][1]] = '#'

# then we do a BFS to search the shortest path from (0,0) to (map_size-1, map_size-1)
def shortest_path(start = (0, 0), end = (map_size - 1, map_size - 1)):

    costs = defaultdict(lambda: float('inf'))
    costs[start] = 0
    Q = []
    heappush(Q, (0, start))
    while Q :
        current_score, current_pos = heappop(Q)

        # searching for the shortest path
        if current_score > costs[current_pos]:
            continue

        # if we are at the end, we look if it's a short path 
        if current_pos == end:
            if current_score < costs[end]:
                costs[end] = current_score
                continue
        
        # for each point we look in every direction to find the shortest path
        for dx, dy in directions:
            new_pos = (current_pos[0] + dx, current_pos[1] + dy)

            # position is a wall or outside the grid
            if not is_within_bounds(new_pos[0], new_pos[1]) or map[new_pos[0]][new_pos[1]] == '#':
                continue

            # being at this point is shorter than the previous one
            if current_score+1 < costs[new_pos] : 
                costs[new_pos] = current_score+1
                heappush(Q, (current_score+1, new_pos))

    return costs[end]


# print the answer
print("Answer:", shortest_path())
# we found 22 for the test18.txt file as expected