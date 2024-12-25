# Opening the file in read mode
with open('input14.txt', 'r') as file:
    input = file.read().strip().split('\n')
input = [robot.split(' ') for robot in input]

robots = []
for robot in input:
    robot_point = robot[0].split(',')
    robot_point_x = int(robot_point[0][2:])
    robot_point_y = int(robot_point[1])

    robot_velocity = robot[1].split(',')
    robot_velocity_x = int(robot_velocity[0][2:])
    robot_velocity_y = int(robot_velocity[1])

    robots.append([robot_point_x, robot_point_y, robot_velocity_x, robot_velocity_y])


# the number of seconds the robots needs to move 
number_of_loops_part1 = 100
# the size of the room, a robot passing on the other side by teleporting (modulo action)
x_room = 101 # 11
y_room = 103 # 7

for i in range(number_of_loops_part1):
    for robot in robots:
        robot[0] += robot[2]
        robot[1] += robot[3]
        robot[0] = robot[0] % x_room
        robot[1] = robot[1] % y_room

def is_overlapping(robots) :
    for i in range(len(robots)):
        for j in range(i+1, len(robots)):
            if robots[i][0] == robots[j][0] and robots[i][1] == robots[j][1]:
                return True
    return False

# taking it where we left it after the first part
number_of_loops_part2 = 100
while is_overlapping(robots) : 
    for robot in robots:
        robot[0] += robot[2]
        robot[1] += robot[3]
        robot[0] = robot[0] % x_room
        robot[1] = robot[1] % y_room
    number_of_loops_part2 += 1


# just to see the christmas see printing
robot_count = [[0 for i in range(x_room)] for j in range(y_room)]
for robot in robots:
    robot_count[robot[1]][robot[0]] += 1
for i in range(y_room):
    for j in range(x_room):
        if robot_count[i][j] > 0:
            print('#', end='')
        else:
            print('.', end='')
    print()

print("Answer: ", number_of_loops_part2)
# we see a christmas priniting in the console