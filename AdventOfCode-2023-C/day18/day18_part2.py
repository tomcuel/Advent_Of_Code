# Open the file called "input.txt" in the same directory as this file.
input_file = open("input18.txt", 'r')
# Read the file into a list where every element is a line from the file in order.
# The lines are stored as strings.
input = input_file.read().strip().split('\n')
# Close the file before we start solving.
input_file.close()


directions = {'U':(-1,0), 'D':(1,0), 'L':(0,-1), 'R':(0,1)}

verts = [(0,0)]
boundary_points = 0
for line in input:
    _, _, hex = line.split()
    hex = hex[2:-1]
    print(hex)
    dir = int(hex[-1])
    dist = int(hex[:-1],16)
    dir = directions["RDLU"[dir]]
    y,x = verts[-1]
    boundary_points += dist
    next = (y + dir[0]*dist, x + dir[1]*dist)
    verts.append(next)

area = 0
for i in range(len(verts)):
    xi = verts[i][0]
    y_im1 = verts[i-1][1]
    y_ip1 = verts[(i+1) % len(verts)][1]
    area += xi* (y_ip1 - y_im1)
area = abs(area) // 2
print(boundary_points)
print("area:",area -  (boundary_points // 2) + boundary_points + 1)


