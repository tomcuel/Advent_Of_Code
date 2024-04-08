from collections import defaultdict


# here we want to count the number of pixels that are on
# to do so, we will do it not by brute force, but by figuring out the number of pixels that are on in each cube of the input 
# the difficulty here is to find the intersection of two cubes and deals with them properly

# parse the input
with open("input22.txt") as fin:
    raw_data = fin.read().strip().split("\n")

instructions = []
for line in raw_data:
    parts = line.split(" ")
    # getting the command
    switch = parts[0] == "on"
    # getting the cibe considered in this command
    cube = []
    for axis in parts[1].split(","):
        axis = axis.split("..")
        cube.append((int(axis[0][2:]), int(axis[1])))

    instructions.append((switch, tuple(cube)))


# function to calculate the volume of a cube
def volume(cube):
    p = 1
    for b in cube:
        assert b[1] >= b[0]
        p *= abs(b[1] - b[0]) + 1
    return p


# function to find the intersection of two cubes
def overlap(cube1, cube2):
    ans = []
    for b1, b2 in zip(cube1, cube2):
        if b1[1] < b2[0] or b2[1] < b1[0]:
            return None

        cube = (max(b1[0], b2[0]), min(b1[1], b2[1]))
        ans.append(cube)

    return tuple(ans)


# count_cube represent if the cube is on or off and the number of times it is on or off
count_cube = defaultdict(int)
# loop through the instructions
for i in range(len(instructions)):
    # get the switch and the cube we're considering here
    switch, cube = instructions[i]

    # create a new dictionary to store the new count link only to the current cube
    new_count_cube = defaultdict(int)
    # key is here to go only once through the count_cube
    keys = set(count_cube.keys())

    for o_cube in keys:
        # check if the cube is on or off
        o_switch = count_cube[o_cube] > 0
        # search for the intersection of the two cubes
        o = overlap(cube, o_cube)
        # if there is no intersection, we continue
        if o == None:
            continue
        
        # Otherwise, we need to reset the new_count to 0
        new_count_cube[o] -= count_cube[o_cube] 

    # if it's on, we increment the count of the cube
    if switch:
        new_count_cube[cube] += 1

    # update the count_cube with the new_count_cube
    for c in new_count_cube:
        count_cube[c] += new_count_cube[c]

ans = 0
# getting the number of pixels that are on
for cube in count_cube:
    ans += volume(cube) * count_cube[cube]
print(ans)
