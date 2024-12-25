# Opening the file in read mode
with open('input6.txt', 'r') as file:
    # splitting the pages inequalities and the upgrade lectures
    map = file.read().strip().split('\n')
map = [list(row) for row in map]
n= len(map)
m = len(map[0])

# Iterate through rows and columns to find the "^"
found = False
for i in range(n):
    for j in range(m):
        if map[i][j] == "^":
            position = (i, j)
            found = True
            break
    if found:
        break

current_direction = 0
directions = [ [-1, 0], [0, 1], [1, 0], [0, -1] ] # ordered by turning 90 degrees clockwise

number_of_loops = 0
# placing a blocker "#" at a position in the map (just not the starting position)
# putting a blocker at an already blocked position will not create a loop so it's not necessary 
# and see if it creates any loop
# we need to then count the number of # we can place to create a loop
for i in range(n):
    for j in range(m):
        # placing a blocker "#" at a position in the map (just not the starting position and not a blocker position)
        if map[i][j] == "." and (i,j) != position:
            map[i][j] = "#"
            creating_a_loop = False

            # A loop is caracterized by an already visited position with an associated direction
            visited_states = set() 
            # creating new variables to not modify the original ones, and still use them in the loop
            current_position = position
            direction = current_direction
            # variable to know if we're creating a loop
            creating_a_loop = False

            # Simulate guard movement as in part 1
            while True:
                
                # getting the next position by adding the direction to the current position
                next_position = (current_position[0] + directions[direction][0],current_position[1] + directions[direction][1])

                # looking if the next position is out of the map
                # it means that there is no loop
                if not (0 <= next_position[0] < n and 0 <= next_position[1] < m):
                    break

                # looking if the next position is a blocker "#", then we need to change the direction
                # the current position doesn't change, but the direction for the next round does
                if map[next_position[0]][next_position[1]] == "#":
                    direction = (direction + 1) % 4
                # otherwise we move to the next position
                else:
                    current_position = next_position
        
                # Check if we return to a previous state (position + direction) to see if we're creating a loop
                state = (current_position, direction)
                if state in visited_states:
                    creating_a_loop = True
                    break
                # Add the current state to the visited states
                visited_states.add(state)

            # Count the loop
            if creating_a_loop:
                number_of_loops += 1

            # Remove the blocker
            map[i][j] = "."


# printing the answer
print("Answer: ", number_of_loops)
# we found 6 for the test6.txt file as expected