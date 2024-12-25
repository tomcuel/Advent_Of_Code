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
number_of_loops = 100
# the size of the room, a robot passing on the other side by teleporting (modulo action)
x_room = 101 # 11
y_room = 103 # 7

for i in range(number_of_loops):
    for robot in robots:
        robot[0] += robot[2]
        robot[1] += robot[3]
        robot[0] = robot[0] % x_room
        robot[1] = robot[1] % y_room

# now we need to count the number of robots in position of the room 
robot_count = [[0 for i in range(x_room)] for j in range(y_room)]
for robot in robots:
    robot_count[robot[1]][robot[0]] += 1

# we need to count the number of robots in each of the 4 quadrants
x_quadrant_size = x_room // 2
y_quadrant_size = y_room // 2

quadrant_count = [[0 for i in range(2)] for j in range(2)]
for i in range(y_room):
    for j in range(x_room):
        number_robot = robot_count[i][j]
        if i < y_quadrant_size and j < x_quadrant_size:
            quadrant_count[0][0] += number_robot
        elif i < y_quadrant_size and j > x_quadrant_size:
            quadrant_count[0][1] += number_robot
        elif i > y_quadrant_size and j < x_quadrant_size:
            quadrant_count[1][0] += number_robot
        elif i > y_quadrant_size and j > x_quadrant_size:
            quadrant_count[1][1] += number_robot

answer = 1
for i in range(2):
    for j in range(2):
        answer *= quadrant_count[i][j]
# print the answer
print("Answer: ", answer)
# we found 12 for the test14.txt file as expected for x_room = 11 and y_room = 7