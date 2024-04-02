from collections import defaultdict
from heapq import heappop, heappush, heapify

# open the file and read the content
with open("input15.txt") as file:
    data=file.read().strip()

map=[[int(x) for x in line] for line in data.split("\n")]

# print(map) to check the map
# for line in map:
    # print(line)

N = len(map)
M = len(map[0])

# now the map is 5*5 times bigger
N_real = N*5
M_real = M*5

# function that get the value in the big map by going to the input map by making some conversion
def get_map(i,j):
    x= (map[i%N][j%M] + (i//N) + (j//M)) # taking into account the number of increments, how far we are in the map from the original one
    return (x-1) %9 +1


# cost of the path that has stopped at the node
costs = defaultdict(int)

# set of visited nodes
visited = set() 

# directions of the possible moves
directions=[(0,1),(0,-1),(1,0),(-1,0)]

# doing the stack 
# the first point as a cost of 0 
heap = [(0,0,0)]
heapify(heap)

# doing the DFS
while len(heap) > 0:
    # taking the next data we will work on 
    cost, i, j = heappop(heap)
    # if the node is alrealdy visited, then skip
    if (i,j) in visited:
        continue
    # otherwise, add the node to the visited set, so that we don't visit it again
    visited.add((i,j))

    # update the cost of the path that has stopped at the node
    costs[(i,j)] = cost

    # if we reach the end, then break
    if i == N_real-1 and j == M_real-1:
        break

    # check the possible moves
    for di, dj in directions:
        ni, nj = i+di, j+dj
        # if the node is out of the map, then skip
        if not (0 <= ni < N_real and 0 <= nj < M_real):
            continue
        
        # if in the map, we can search further
        heappush(heap, (cost+get_map(ni,nj), ni, nj))


print("The answer is: ", costs[(N_real-1,M_real-1)])


# The answer is 315 as expected for test.txt
# It's way easier here than in C because the library didn't provide me heap, stacks that could take multiple values
# I tried to do it, by having multpiles of them but it didn't work for me 
# DFS was difficult to implement in C with stacks and I couldn't find a way to do it with heaps (because It sorted things and I needed to keep the order of the nodes as I also have their value to take into account)
# The next solution wasn't very working, even if I kinda work