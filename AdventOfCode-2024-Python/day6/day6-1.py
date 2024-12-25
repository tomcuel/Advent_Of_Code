# Opening the file in read mode
with open('input6.txt', 'r') as file:
    # splitting the pages inequalities and the upgrade lectures
    map = file.read().strip().split('\n')
n= len(map)
m = len(map[0])

# Iterate through rows and columns to find the "^"
for row_idx, row in enumerate(map):
    col_idx = row.find("^")  # Returns -1 if "^" is not in the row
    if col_idx != -1:
        position = (row_idx, col_idx)
        break

current_direction = 0
directions = [ [-1, 0], [0, 1], [1, 0], [0, -1] ] # ordered by turning 90 degrees clockwise

# Initialize the map of the point we've visited
visited = set()
# iterating while the position is not going out of the map 
while True : 

    # adding the current position to the visited points
    visited.add(position)

    # getting the next position by adding the direction to the current position
    next_position = (position[0] + directions[current_direction][0], position[1] + directions[current_direction][1])

    # looking if the next position is out of the map
    if not (0 <= next_position[0] < n and 0 <= next_position[1] < m):
        break

    # looking if the next position is a blocker "#", then we need to change the direction
    # the current position doesn't change, but the direction for the next round does
    if map[next_position[0]][next_position[1]] == "#":
        current_direction = (current_direction + 1) % 4
    # otherwise we move to the next position
    else : 
        position = next_position

# the answer is the number of 1 in visited points
print("Answer: ", len(visited))
# we found 41 for the test6.txt file as expected