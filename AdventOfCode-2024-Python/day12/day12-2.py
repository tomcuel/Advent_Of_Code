from collections import defaultdict, deque

# Opening the file in read mode
with open('input12.txt', 'r') as file:
    lines = file.read().strip().split('\n')
lines = [[c for c in line] for line in lines]
# function to check if the position is within the bounds of the grid
n = len(lines)
m = len(lines[0])
def is_within_bounds(x, y):
    return x >= 0 and x < n and y >= 0 and y < m

directions = [(1, 0), (-1, 0), (0, 1), (0, -1)]

p1 = 0
p2 = 0
visited = set() # so we don't visit the same point twice
for i in range(n):
    for j in range(m):
        if (i,j) in visited :
            continue
        queue = deque([(i,j)])
        area = 0
        perimeter = 0
        perimeter_dict = dict() # for each direction, for the point, it gives us the points that are on a border
        while queue:
            x, y = queue.popleft()
            if (x, y) in visited:
                continue
            visited.add((x, y))
            area += 1
            for dx, dy in directions:
                xx, yy = x + dx, y + dy
                if is_within_bounds(xx, yy) and lines[xx][yy] == lines[x][y]:
                    queue.append((xx, yy))
                else : 
                    # otherwise, we are at the border of the block
                    perimeter += 1
                    # until here, I did the same as part 1
                    # we keep track of the border we've check for a certain direction
                    if (dx, dy) not in perimeter_dict : 
                        perimeter_dict[(dx, dy)] = set()
                    perimeter_dict[(dx, dy)].add((x,y))
        # couting the sides of the perimeter
        sides = 0 
        for _, vs in perimeter_dict.items(): # vs is a direction
            visited_perimeter = set()
            for vx, vy in vs:
                if (vx, vy) not in visited_perimeter:
                    sides += 1 # we found a side, then we need to make sure to eliminate all the points that are on the same side, not to count them again
                    queue = deque([(vx, vy)])
                    while queue:
                        x, y = queue.popleft()
                        if (x, y) in visited_perimeter:
                            continue
                        visited_perimeter.add((x, y))
                        for dx, dy in directions:
                            xx, yy = x + dx, y + dy
                            if (xx, yy) in vs:
                                queue.append((xx, yy))
        
        p1 += area * perimeter
        p2 += area * sides        


# print the answer
print("Answer P1:", p1)
print("Answer P2:", p2)
# we found 80 for the test12_1.txt file as expected
# we found 436 for the test12_2.txt file as expected
# we found 1206 for the test12.txt file as expected