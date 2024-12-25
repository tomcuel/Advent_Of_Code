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
got_the_base_time = False
base_time = 0
for i in range(len(cheat_possibility)+1):
    # we can consider the cheats after getting the base time
    if got_the_base_time:
        # introduce the cheat passage
        racetrack[cheat_possibility[i-1][0]][cheat_possibility[i-1][1]] = 'C'
    queue = [(start[0], start[1], 0)]
    visited = set()
    visited.add((start[0], start[1]))
    while queue:
        x, y, steps = queue.pop(0)
        # reaching the end 
        if (x, y) == end:
            if not got_the_base_time:
                base_time = steps
            else :
                cheats.append((cheat_possibility[i-1][0], cheat_possibility[i-1][1], steps))
            break
        for dx, dy in directions:
            new_x, new_y = x + dx, y + dy
            if is_within_bounds(new_x, new_y) and (new_x, new_y) not in visited and racetrack[new_x][new_y] in '.EC':
                visited.add((new_x, new_y))
                queue.append((new_x, new_y, steps + 1))
    
    # we can consider the cheats after getting the base time
    if got_the_base_time:
        # remove the current cheat passage
        racetrack[cheat_possibility[i-1][0]][cheat_possibility[i-1][1]] = '#'
    else:
        got_the_base_time = True

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