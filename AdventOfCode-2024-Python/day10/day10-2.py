# Opening the file in read mode
with open('input10.txt', 'r') as file:
    map = file.read().strip().split('\n')
map = [[int(x) for x in row] for row in map]
n = len(map)

directions = [[0,1], [0,-1], [1,0], [-1,0]]

def is_valid(x, y):
    return 0 <= x < n and 0 <= y < n

# from each 0 we search a path to a 1-2-3-4-5-6-7-8-9 combination

# get the number of hiking path from the (x, y) point 
# doing a DFS
def score(x, y):

    # we only start from a 0
    if map[x][y] != 0:
        return 0
    
    number_of_hiking_trails = 0
    stack_in_wait = [(x,y)]
    while len(stack_in_wait) > 0:
        current_x, current_y = stack_in_wait.pop()
        current_value = map[current_x][current_y]

        # there is no visited set, we can go back to the same summit 
        # it's why we want a ranking, so that we know by how many trails we can go to the same summit

        if current_value == 9 : 
            number_of_hiking_trails += 1
            continue

        for dx, dy in directions:
            new_x, new_y = current_x + dx, current_y + dy
            if not is_valid(new_x, new_y):
                continue

            new_value = map[new_x][new_y]
            # looking if it's the next number
            if new_value != current_value + 1:
                continue

            stack_in_wait.append((new_x, new_y))

    return number_of_hiking_trails

# iterate over all the cells
answer = 0
for i in range(n):
    for j in range(n):
        answer += score(i,j)

# print the answer
print("Answer:", answer)
# we found 81 for the test10.txt file as expected