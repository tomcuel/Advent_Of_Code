from heapq import heappush, heappop
from collections import defaultdict

# Opening the file
with open('input20.txt', 'r') as file:
    racetrack = file.read().strip().split('\n')
racetrack = [list(row) for row in racetrack]
n = len(racetrack)
m = len(racetrack[0])


# Utility functions
def is_within_bounds(x, y):
    return 0 <= x < n and 0 <= y < m


points = set()
start = None
end = None
# Finding the starting point and ending point, and all the already possible points
for i in range(len(racetrack)):
    for j in range(len(racetrack[i])):
        if racetrack[i][j] == 'S':
            start = (i, j)
            points.add((i, j))
        elif racetrack[i][j] == 'E':
            end = (i, j)
            points.add((i, j))
        elif racetrack[i][j] == '.':
            points.add((i, j))


# function to get all the points that are within d cases from a point 
# using the manhattan distance : will be used to get the point reachable from 20points 
def get_point_in_distance(point, d):
    d_points = set()
    # possible points within d cases from the point
    for dx in range(-d, d + 1):
        # Remaining distance for y given dx
        dy_range = d - abs(dx)  
        for dy in range(-dy_range, dy_range + 1):
            d_points.add((point[0] + dx, point[1] + dy))
    return d_points


# a cheat can happens at any time during 20 consecutive picoseconds in the second part 
length_cheat = 20
second_to_save = 100 # 50 for the test20.txt and 100 for the input20.txt


# return the lowest cost to go from start to end 
def find_lowest_cost(points, start, end) : 
    costs = defaultdict(lambda: 1 << 20)
    costs[start], queue = 0, []
    heappush(queue, (start, 0))
    while queue : 
        current_pos, current_cost = heappop(queue)
        if current_cost > costs[current_pos] : 
            continue # bad path

        if current_pos == end : 
            if current_cost < costs[end] : # new best path
                costs[end] = current_cost
            continue

        for new_point in get_point_in_distance(current_pos, 1) : 
            if new_point not in points : 
                continue # need to be reachable, on the path

            if current_cost+1 < costs[new_point] : # new best path
                costs[new_point] = current_cost + 1
                heappush(queue, (new_point, current_cost+1))

    # getting the path
    return costs[end]


# function to find the path the cost and the path for the problem without any cheating
# the difference here is that we're considering the path, it will allow use to be able to : 
# - know where a cheat ends up on the basic path 
# - we will be able to know how much of the past is still left for us 
# - it needs to take into account the predecesseurs for each point
def find_path_and_costs_per_point_without_cheat(points, start, end) : 
    costs = defaultdict(lambda: 1 << 20)
    predecesseurs = {}
    costs[start], queue = 0, []
    heappush(queue, (start, 0))
    while queue : 
        current_pos, current_cost = heappop(queue)
        if current_cost > costs[current_pos] : 
            continue # bad path

        if current_pos == end : 
            if current_cost < costs[end] : # new best path
                costs[end] = current_cost
                predecesseurs[end] = current_pos
            continue

        for new_point in get_point_in_distance(current_pos, 1) : 
            if new_point not in points :  
                continue # need to be reachable, on the path

            if current_cost+1 < costs[new_point] : # new best path
                costs[new_point] = current_cost + 1
                predecesseurs[new_point] = current_pos
                heappush(queue, (new_point, current_cost+1))

    # getting the path
    path = {end}
    pred = end
    while pred in predecesseurs :
        path.add(predecesseurs[pred])
        pred = predecesseurs[pred]
    return costs, path


def get_answer(points, start, end) : 
    # getting the basic path and costs 
    costs, path = find_path_and_costs_per_point_without_cheat(points, start, end)

    answer = 0
    results = defaultdict(int)
    # then we add a cheat for each point in the path 
    for point in path : 
        for cheat_point in get_point_in_distance(point, length_cheat) : 
            if cheat_point not in points :
                continue

            # we have the cost to go from the start and to the p point 
            # we can calculate the cost adding this point m will do
            # and we know thanks to the manhattan distance the cost to go from p to m

            if cheat_point in path : # we already know the value of a point of the path and its cost at the point
                cheat_cost = costs[cheat_point]
            else : 
                cheat_cost = find_lowest_cost(points, cheat_point, end) # getting the path to go from m to the end --> as part 1 BFS here 
            cheat_distance_cost = abs(cheat_point[0]-point[0]) + abs(cheat_point[1]-point[1])
            # we can now get how many second we cheated
            # cost to go from start to point - (cost to go from m to end + cost to go from point to m)
            saved_cost = costs[point] - (cheat_cost + cheat_distance_cost)
            if saved_cost >= second_to_save : 
                answer += 1
                results[saved_cost] += 1

    return answer, results

# getting the answer
answer, results = get_answer(points, start, end)
'''
for key in sorted(results.keys()) : 
    print(key, results[key])
just to test the function for the test20.txt file : normal path is 84
and we save for a potnetial cheat of 20ps max : 
- 50ps : 32 times 
- 52 : 31 times
- 54 : 29 times
- 56 : 39 times
- 58 : 25 times
- 60 : 23 times
- 62 : 20 times
- 64 : 19 times
- 66 : 12 times
- 68 : 14 times
- 70 : 12 times
- 72 : 22 times
- 74 : 4 times
- 76 : 3 times
'''

# print the answer 
print("Answer : ", answer)
# took me so much time to finger out how to do the second part
# I did look for some solution to avoid computing the path for each point, and the things with the cost and path memoization helped me a lot