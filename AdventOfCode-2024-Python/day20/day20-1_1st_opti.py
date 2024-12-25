from collections import deque

# Opening the file
with open('input20.txt', 'r') as file:
    racetrack = file.read().strip().split('\n')
racetrack = [list(row) for row in racetrack]
n = len(racetrack)
m = len(racetrack[0])

# Utility functions
directions = [(0, 1), (1, 0), (0, -1), (-1, 0)]
def is_within_bounds(x, y):
    return 0 <= x < n and 0 <= y < m

# Finding the starting point and ending point
for i in range(len(racetrack)):
    for j in range(len(racetrack[i])):
        if racetrack[i][j] == 'S':
            start = (i, j)
        if racetrack[i][j] == 'E':
            end = (i, j)

# Bidirectional BFS for base time and shortest path
def bidirectional_bfs(start, end):
    queue_start = deque([(start[0], start[1], 0)])
    queue_end = deque([(end[0], end[1], 0)])

    visited_start = [[False for _ in range(m)] for _ in range(n)]
    visited_end = [[False for _ in range(m)] for _ in range(n)]
    visited_start[start[0]][start[1]] = True
    visited_end[end[0]][end[1]] = True

    dist_start = {(start[0], start[1]): 0}
    dist_end = {(end[0], end[1]): 0}

    while queue_start or queue_end:
        # Expand from start
        if queue_start:
            x, y, steps = queue_start.popleft()
            for dx, dy in directions:
                new_x, new_y = x + dx, y + dy
                if is_within_bounds(new_x, new_y) and not visited_start[new_x][new_y] and racetrack[new_x][new_y] in '.EC':
                    visited_start[new_x][new_y] = True
                    queue_start.append((new_x, new_y, steps + 1))
                    dist_start[(new_x, new_y)] = steps + 1
                    if (new_x, new_y) in dist_end:
                        return dist_start[(new_x, new_y)] + dist_end[(new_x, new_y)]

        # Expand from end
        if queue_end:
            x, y, steps = queue_end.popleft()
            for dx, dy in directions:
                new_x, new_y = x + dx, y + dy
                if is_within_bounds(new_x, new_y) and not visited_end[new_x][new_y] and racetrack[new_x][new_y] in '.EC':
                    visited_end[new_x][new_y] = True
                    queue_end.append((new_x, new_y, steps + 1))
                    dist_end[(new_x, new_y)] = steps + 1
                    if (new_x, new_y) in dist_start:
                        return dist_start[(new_x, new_y)] + dist_end[(new_x, new_y)]

    return float('inf')  # If no path exists

# getting the base time of the problem to calculate the time saved by the cheats
base_time = bidirectional_bfs(start, end)

# a wall that can be broken to introduce a cheat is a wall '#' surrounded by 1 '.' on each side 
# instead of try to test all the wall, we will test only the wall where there is a '.w.' pattern 
cheat_possibility = []
for i in range(n):
    for j in range(m):
        if racetrack[i][j] == '#':
            # vertical cheat passage
            if is_within_bounds(i-1, j) and is_within_bounds(i+1, j) and racetrack[i-1][j] in '.ES' and racetrack[i+1][j] in '.ES':
                cheat_possibility.append((i, j))
            # horizontal cheat passage
            if is_within_bounds(i, j-1) and is_within_bounds(i, j+1) and racetrack[i][j-1] in '.ES' and racetrack[i][j+1] in '.ES':
                cheat_possibility.append((i, j))

# now we find the number of picosecond to reach the end (number of steps)
# by introducing a cheat passage at each possible location
cheats = []
for cheat_x, cheat_y in cheat_possibility :
    # Introduce the cheat passage
    racetrack[cheat_x][cheat_y] = 'C'
    time_with_cheat = bidirectional_bfs(start, end)
    cheats.append((cheat_x, cheat_y, time_with_cheat))
    # Remove the cheat passage
    racetrack[cheat_x][cheat_y] = '#'

'''
we find for the test20.txt file : normal path is 84
# for (1,8) : 72
# for (7,10) : 64
# for (8;8) : 46
# for (7,6) : 20
'''
save_time_array = []
for i in range(len(cheats)):
    save_time_array.append(base_time - cheats[i][2])
save_time_array.sort(key=lambda x: x, reverse=True)
'''
we found the saving times things for the test19.txt file
# save 14x2ps
# save 14x4ps
# save 2x6ps
# save 4x8ps
# save 2x10ps
# save 3x12ps
# save 1x20ps
# save 1x36ps
# save 1x38ps
# save 1x40ps
# save 1x64ps
'''
answer = 0
for i in range(len(save_time_array)):
    if save_time_array[i] >= 100:
        answer += 1
    else:
        break
# print the answer
print("Answer:", answer)
# we found 0 for the test20.txt file as expected
# changing the BFS to make it faster, from 1min40s to 50s