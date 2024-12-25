from collections import deque

# Opening the file
with open('input16.txt', 'r') as file:
    map = file.read().strip().split('\n')
map = [list(row) for row in map]
n = len(map)
m = len(map[0])

def is_within_bounds(x, y):
    return 0 <= x < n and 0 <= y < m

# getting the start and end points
x_start, y_start = 0, 0
x_end, y_end = 0, 0
for i in range(n):
    for j in range(m):
        if map[i][j] == 'S':
            x_start, y_start = i, j
        if map[i][j] == 'E':
            x_end, y_end = i, j

# directions ordered as: right, down, left, up since a change in direction is count as a 1000 points penalty
dx = [0, 1, 0, -1]
dy = [1, 0, -1, 0]

# function to calculate the penalty points to get to the end point
def calculate_penalty_points(x,y) : 
    # matrix of the cost to get to a point (x,y) of the map given a direction from which we arrived
    cost_per_point = [[[1000000 for _ in range(4)] for _ in range(m)] for _ in range(n)] 
    priority_queue = deque() # priority queue to store the points to visit for the next iteration

    cost_per_point[x][y][0] = 0
    priority_queue.append((0, x, y, 0)) # add the start point to the priority queue, starting looking at the east direction (0,1)
    
    while priority_queue:
        # sort the priority queue by the score of the points
        priority_queue = deque(sorted(priority_queue, key=lambda k: k[0])) # sort the priority queue by the score of the points
        score, x, y, direction = priority_queue.popleft()

        # if the cost to get to the point (x,y) from the direction we arrived is not the same as the score, we skip the point
        if cost_per_point[x][y][direction] != score:
            continue

        # add the points neifghbors to the priority queue depending on the direction we go, it can change clockwise or counterclockwise
        # if turning clockwise costs less than going straight, we turn clockwise
        if cost_per_point[x][y][(direction + 1) % 4] > score + 1000:
            cost_per_point[x][y][(direction + 1) % 4] = score + 1000
            priority_queue.append((score + 1000, x, y, (direction + 1) % 4))
        # if turning counterclockwise costs less than going straight, we turn counterclockwise
        if cost_per_point[x][y][(direction - 1) % 4] > score + 1000:
            cost_per_point[x][y][(direction - 1) % 4] = score + 1000
            priority_queue.append((score + 1000, x, y, (direction - 1) % 4))

        # get the coordinates of the next point
        next_x, next_y = x + dx[direction], y + dy[direction]
        # if the next point is out of bounds or is a wall or the cost to get to the next point from the direction we arrived is less than the score + 1, we skip the point
        if not is_within_bounds(next_x, next_y) or map[next_x][next_y] == '#' or cost_per_point[next_x][next_y][direction] <= score + 1:
            continue

        # update the cost to get to the next point from the direction we arrived and add the point to the priority queue
        cost_per_point[next_x][next_y][direction] = score + 1
        priority_queue.append((score + 1, next_x, next_y, direction))

    return cost_per_point


cost_array = calculate_penalty_points(x_start, y_start)
answer = min(cost_array[x_end][y_end]) # get the minimum score to get to the end point (depending on the direction we arrived)
# print the answer
print("Answer:", answer)
# we found 7036 for the test16_1.txt file as expected
# we found 11048 for the test16_2.txt file as expected