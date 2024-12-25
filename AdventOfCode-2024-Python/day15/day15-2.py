# Opening the file in read mode
with open('input15.txt', 'r') as file:
    input = file.read().strip().split('\n\n')
map = input[0].split('\n')
map = [list(row) for row in map]
n = len(map)
m = len(map[0])
command = ''.join(input[1].split('\n'))

# modifying the map as wanted in the puzzle
'''
- If the tile is #, the new map contains ## instead.
- If the tile is O, the new map contains [] instead.
- If the tile is ., the new map contains .. instead.
- If the tile is @, the new map contains @. instead.
'''
new_map = [['' for _ in range(2*m)] for _ in range(n)]
for i in range(n):
    for j in range(m):
        if map[i][j] == '#':
            new_map[i][2*j] = '#'
            new_map[i][2*j+1] = '#'
        elif map[i][j] == 'O':
            new_map[i][2*j] = '['
            new_map[i][2*j+1] = ']'
        elif map[i][j] == '.':
            new_map[i][2*j] = '.'
            new_map[i][2*j+1] = '.'
        elif map[i][j] == '@':
            new_map[i][2*j] = '@'
            new_map[i][2*j+1] = '.'
n = len(new_map)
m = len(new_map[0])

directions = {'<': (0, -1), '>': (0, 1), '^': (-1, 0), 'v': (1, 0)}

# getting the robot starting position
def find_robot_position(map):
    robot = (-1,-1)
    found = False
    for i in range(len(map)):
        for j in range(len(map[i])):
            if map[i][j] == '@':
                robot = (i,j)
                found = True
                break
        if found:
            break
    return robot

k = len(command)
for i in range(k):
    dir = directions[command[i]]
    robot = find_robot_position(new_map)

    # we look if in the direction we're moving we can push a whole line of '[]' blocks into this direction
    # if we can't, nothing move 
    # if we can, we move the robot and the blocks
    # counting the number of '[' or ']' in the direction we're moving until we hit a wall or a block
    # we also count the distance to the next wall or block

    # counting the distance between the robot and the next wall or block
    d_from_block = 0
    while new_map[robot[0] + (d_from_block+1)*dir[0]][robot[1] + (d_from_block+1)*dir[1]] != '#' : 
        d_from_block += 1
    # counting the number of '0' blocks in the direction we're moving until we have something else ("#" or ".")
    nb_boxe_horizontally = 0
    nb_boxe_vertically = 0
    if dir == (0, 1) or dir == (0, -1) :
        while new_map[robot[0] + (nb_boxe_horizontally+1)*dir[0]][robot[1] + (nb_boxe_horizontally+1)*dir[1]] == '[' or new_map[robot[0] + (nb_boxe_horizontally+1)*dir[0]][robot[1] + (nb_boxe_horizontally+1)*dir[1]] == ']' : 
            nb_boxe_horizontally += 2
    elif  dir == (1, 0) or dir == (-1, 0) : 
        while new_map[robot[0] + (nb_boxe_vertically+1)*dir[0]][robot[1] + (nb_boxe_vertically+1)*dir[1]] == '[' or new_map[robot[0] + (nb_boxe_vertically+1)*dir[0]][robot[1] + (nb_boxe_vertically+1)*dir[1]] == ']' :
            nb_boxe_vertically += 1 
    
    can_move = True            
    to_move = []

    if d_from_block > 0 and nb_boxe_horizontally < d_from_block and nb_boxe_vertically < d_from_block : 
        if nb_boxe_horizontally > 0 :
            if dir[1] > 0 :
                to_move = new_map[robot[0]][(robot[1] + dir[1]):(robot[1] + nb_boxe_horizontally+1)]
                for j in range(len(to_move)):
                    new_map[robot[0]][robot[1] + (j+2)*dir[1]] = to_move[j]
            elif dir[1] < 0 :
                to_move = new_map[robot[0]][(robot[1] - nb_boxe_horizontally):(robot[1] + dir[1] + 1)]
                for j in range(len(to_move)):
                    new_map[robot[0]][robot[1] + (j+2)*dir[1]] = to_move[len(to_move)-1 - j]
        elif nb_boxe_vertically > 0 : 
            
            if dir[0] > 0 :
                # getting the columns elements to move and their boxes corresponding ones
                for k in range(robot[0]+1,(robot[0] + nb_boxe_vertically+1)):
                    to_move.append((k, robot[1], new_map[k][robot[1]]))
                    if new_map[k][robot[1]] == '[' :
                        to_move.append((k, robot[1]+1, new_map[k][robot[1]+1]))
                    elif new_map[k][robot[1]] == ']' :
                        to_move.append((k, robot[1]-1, new_map[k][robot[1]-1]))
            elif dir[0] < 0 :
                # getting the columns elements to move and their boxes corresponding ones
                for k in range((robot[0] - nb_boxe_vertically),robot[0]):
                    to_move.append((k, robot[1], new_map[k][robot[1]]))
                    if new_map[k][robot[1]] == '[' :
                        to_move.append((k, robot[1]+1, new_map[k][robot[1]+1]))
                    elif new_map[k][robot[1]] == ']' :
                        to_move.append((k, robot[1]-1, new_map[k][robot[1]-1]))
            
            delta = 1
            if dir[0] < 0 :
                delta = -1
            # getting all the elements to move
            stack = [elem for elem in to_move]
            while stack and can_move == True:
                elem = stack.pop()
                # if there is a '#' above we need to tell that this not going to be able to move 
                if new_map[elem[0]+delta][elem[1]] == '#' : 
                    can_move = False
                    to_move = []
                    break
                # since we need to move up the '[' we need to move up the ']' too 
                # and if there is a box above this ']' we need to move it too, so we add the point to check to the stack 
                if (new_map[elem[0]+delta][elem[1]] == '[' or new_map[elem[0]+delta][elem[1]] == ']') and not (elem[0]+delta, elem[1], new_map[elem[0]+delta][elem[1]]) in to_move :
                    stack.append((elem[0]+delta, elem[1], new_map[elem[0]+delta][elem[1]]))
                    to_move.append((elem[0]+delta, elem[1], new_map[elem[0]+delta][elem[1]]))
                    # looking right to it 
                    if new_map[elem[0]+delta][elem[1]] == '[' :
                        to_move.append((elem[0]+delta, elem[1]+1, new_map[elem[0]+delta][elem[1]+1]))
                        stack.append((elem[0]+delta, elem[1]+1, new_map[elem[0]+delta][elem[1]+1]))
                    # looking left to it
                    elif new_map[elem[0]+delta][elem[1]] == ']' :
                        to_move.append((elem[0]+delta, elem[1]-1, new_map[elem[0]+delta][elem[1]-1]))
                        stack.append((elem[0]+delta, elem[1]-1, new_map[elem[0]+delta][elem[1]-1]))

            # now we can move the elements down one cell, beggining with the lower ones (row >)
            sorted_to_move = []
            if dir[0] > 0 :
                sorted_to_move = sorted(to_move, key=lambda x: (x[0], x[1]), reverse=True)
            elif dir[0] < 0 :
                sorted_to_move = sorted(to_move, key=lambda x: (x[0], x[1]))
            if can_move :
                for j in range(len(sorted_to_move)):
                    new_map[sorted_to_move[j][0]+delta][sorted_to_move[j][1]] = sorted_to_move[j][2]
                    new_map[sorted_to_move[j][0]][sorted_to_move[j][1]] = '.'

            '''
            if dir[0] > 0 :
                # getting the columns elements to move and their boxes corresponding ones
                for k in range(robot[0]+1,(robot[0] + nb_boxe_vertically+1)):
                    to_move.append((k, robot[1], new_map[k][robot[1]]))
                    if new_map[k][robot[1]] == '[' :
                        to_move.append((k, robot[1]+1, new_map[k][robot[1]+1]))
                    elif new_map[k][robot[1]] == ']' :
                        to_move.append((k, robot[1]-1, new_map[k][robot[1]-1]))

                # getting all the elements to move
                stack = [elem for elem in to_move]
                while stack and can_move == True:
                    elem = stack.pop()
                    # if there is a '#' below we need to tell that this not going to be able to move 
                    if new_map[elem[0]+1][elem[1]] == '#' : 
                        can_move = False
                        to_move = []
                        break
                    # since we need to move down the '[' we need to move down the ']' too 
                    # and if there is a box below this ']' we need to move it too, so we add the point to check to the stack 
                    if (new_map[elem[0]+1][elem[1]] == '[' or new_map[elem[0]+1][elem[1]] == ']') and not (elem[0]+1, elem[1], new_map[elem[0]+1][elem[1]]) in to_move :
                        stack.append((elem[0]+1, elem[1], new_map[elem[0]+1][elem[1]]))
                        to_move.append((elem[0]+1, elem[1], new_map[elem[0]+1][elem[1]]))
                        if new_map[elem[0]+1][elem[1]] == '[' :
                            to_move.append((elem[0]+1, elem[1]+1, new_map[elem[0]+1][elem[1]+1]))
                            stack.append((elem[0]+1, elem[1]+1, new_map[elem[0]+1][elem[1]+1]))
                        elif new_map[elem[0]+1][elem[1]] == ']' :
                            to_move.append((elem[0]+1, elem[1]-1, new_map[elem[0]+1][elem[1]-1]))
                            stack.append((elem[0]+1, elem[1]-1, new_map[elem[0]+1][elem[1]-1]))
                
                # now we can move the elements down one cell, beggining with the lower ones (row >)
                sorted_to_move = sorted(to_move, key=lambda x: (x[0], x[1]), reverse=True)
                print(sorted_to_move)
                if can_move :
                    for j in range(len(sorted_to_move)):
                        new_map[sorted_to_move[j][0]+1][sorted_to_move[j][1]] = sorted_to_move[j][2]
                        new_map[sorted_to_move[j][0]][sorted_to_move[j][1]] = '.'
                
            elif dir[0] < 0 :
                # getting the columns elements to move and their boxes corresponding ones
                for k in range((robot[0] - nb_boxe_vertically),robot[0]):
                    to_move.append((k, robot[1], new_map[k][robot[1]]))
                    if new_map[k][robot[1]] == '[' :
                        to_move.append((k, robot[1]+1, new_map[k][robot[1]+1]))
                    elif new_map[k][robot[1]] == ']' :
                        to_move.append((k, robot[1]-1, new_map[k][robot[1]-1]))
                
                # getting all the elements to move
                stack = [elem for elem in to_move]
                while stack and can_move == True:
                    elem = stack.pop()
                    # if there is a '#' above we need to tell that this not going to be able to move 
                    if new_map[elem[0]-1][elem[1]] == '#' : 
                        can_move = False
                        to_move = []
                        break
                    # since we need to move up the '[' we need to move up the ']' too 
                    # and if there is a box above this ']' we need to move it too, so we add the point to check to the stack 
                    if (new_map[elem[0]-1][elem[1]] == '[' or new_map[elem[0]-1][elem[1]] == ']') and not (elem[0]-1, elem[1], new_map[elem[0]-1][elem[1]]) in to_move :
                        stack.append((elem[0]-1, elem[1], new_map[elem[0]-1][elem[1]]))
                        to_move.append((elem[0]-1, elem[1], new_map[elem[0]-1][elem[1]]))
                        if new_map[elem[0]-1][elem[1]] == '[' :
                            to_move.append((elem[0]-1, elem[1]+1, new_map[elem[0]-1][elem[1]+1]))
                            stack.append((elem[0]-1, elem[1]+1, new_map[elem[0]-1][elem[1]+1]))
                        elif new_map[elem[0]-1][elem[1]] == ']' :
                            to_move.append((elem[0]-1, elem[1]-1, new_map[elem[0]-1][elem[1]-1]))
                            stack.append((elem[0]-1, elem[1]-1, new_map[elem[0]-1][elem[1]-1]))
                
                # now we can move the elements up one cell, beggining with the higher ones (row <)
                sorted_to_move = sorted(to_move, key=lambda x: (x[0], x[1]))
                if can_move :
                    for j in range(len(sorted_to_move)):
                        new_map[sorted_to_move[j][0]-1][sorted_to_move[j][1]] = sorted_to_move[j][2]
                        new_map[sorted_to_move[j][0]][sorted_to_move[j][1]] = '.'
                '''
        if can_move :
            new_map[robot[0]][robot[1]] = '.'
            new_map[robot[0] + dir[0]][robot[1] + dir[1]] = '@'

       
        


for row in new_map:
    print(''.join(row))
'''
finding the good result for both the test15.txt and the test15_2.txt
'''




answer = 0
for i in range(n):
    for j in range(m):
        if new_map[i][j] == '[' : 
            answer += 100 * i + j
# printing the answer
print("Answer:", answer) 
# we found 9021 for the test15.txt file as expected