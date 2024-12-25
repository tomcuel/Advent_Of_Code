from collections import defaultdict #used for dictionary with list
from itertools import combinations #used to get all the combinations of a list of elements, here it will be points

# Opening the file in read mode
with open('input8.txt', 'r') as file:
    # splitting the pages inequalities and the upgrade lectures
    lines = file.read().strip().split('\n')
lines = [[char for char in line] for line in lines]
n = len(lines)
m = len(lines[0])

# function to check if a grid spot is within the grid --> anti nodes can't be outside the grid
def is_within_grid(x, y) : 
    return 0 <= x < n and 0 <= y < m

'''
..........
...#......
..........
....a.....
..........
.....a....
..........
......#...
..........
..........
'''
# an anti node is the # symbol, adding the # at the bottom is as if we we're making the same path as from the top a to the bottom a
# we're looking for diagonals prolonging the path from the top a to the bottom a
# here the antenna have the same symbol on the grid
def antinodes_of_two_same_antennas(P1, P2) : # P1 = (x1, y1), P2 = (x2, y2)
    P1x, P1y = P1
    P2x, P2y = P2

    a1x, a1y = P1x - (P2x - P1x), P1y - (P2y - P1y)
    a2x, a2y = P2x + (P2x - P1x), P2y + (P2y - P1y)

    is_a1_within_grid = is_within_grid(a1x, a1y)
    is_a2_within_grid = is_within_grid(a2x, a2y)
    antenna_antinodes = []
    if is_a1_within_grid : 
        antenna_antinodes.append((a1x, a1y))
    if is_a2_within_grid :
        antenna_antinodes.append((a2x, a2y))
    return antenna_antinodes

# it looks like that for the test8.txt file : 
'''
0 [(1, 8), (2, 5), (3, 7), (4, 4)]
A [(5, 6), (8, 8), (9, 9)]
'''
points_name_and_locations = defaultdict(list)
for i in range(n) :
    for j in range(m) :
        # then it's an antenna
        if lines[i][j] != "." : 
            points_name_and_locations[lines[i][j]].append((i, j))

antinodes = set()
# we're adding 
for antenna_type in points_name_and_locations : 
    antenna_positions = points_name_and_locations[antenna_type]
    for P1, P2 in combinations(antenna_positions, 2) : 
        for antinode in antinodes_of_two_same_antennas(P1, P2) : 
            antinodes.add(antinode)
    

# print the answer
print("Answer:", len(antinodes))
# we found 14 for the test8.txt file as expected