# Opening the file in read mode
with open('input15.txt', 'r') as file:
    input = file.read().strip().split('\n\n')
map = input[0].split('\n')
map = [list(row) for row in map]
command = ''.join(input[1].split('\n'))

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

for i in range(len(command)):
    dir = directions[command[i]]
    robot = find_robot_position(map)

    # we look if in the direction we're moving we can push a whole line of '0' blocks into this direction
    # if we can't, nothing move 
    # if we can, we move the robot and the blocks
    # counting the number of '0' blocks in the direction we're moving until we hit a wall or a block
    # we also count the distance to the next wall or block

    # counting the distance between the robot and the next wall or block
    d_from_block = 0
    while map[robot[0] + (d_from_block+1)*dir[0]][robot[1] + (d_from_block+1)*dir[1]] != '#' : 
        d_from_block += 1
    # counting the number of '0' blocks in the direction we're moving until we have something else ("#" or ".")
    nb_0_to_push = 0
    while map[robot[0] + (nb_0_to_push+1)*dir[0]][robot[1] + (nb_0_to_push+1)*dir[1]] == 'O' : 
        nb_0_to_push += 1

    # to be able to move it we need to have enough distance between the robot + the O and the next wall or block to make the slide
    if d_from_block > 0 and nb_0_to_push < d_from_block:
        map[robot[0]][robot[1]] = '.'
        map[robot[0] + dir[0]][robot[1] + dir[1]] = '@'
        for j in range(2,nb_0_to_push+2) : 
            map[robot[0] + j*dir[0]][robot[1] + j*dir[1]] = 'O'

    '''      
    print("distance from wall or blocks : ", d_from_block)
    print("number of 0 to move : ", nb_0_to_push)
    for row in map:
        print(row)
    print()
    # it does work for test15_1.txt
    '''

'''
print("Final map : ")
for row in map :
    print(row)
# does work for test15.txt
'''
answer = 0
for i in range(len(map)):
    for j in range(len(map[i])):
        if map[i][j] == 'O':
            answer += 100 * i + j
# printing the answer
print("Answer:", answer) 
# we found 10092 for the test15.txt file as expected