# Opening the file
with open('input18.txt', 'r') as file:
    memory_space = file.read().strip().split('\n')
memory_space = [[int (i) for i in line.split(',')] for line in memory_space]
map_size = 71 # 71x71 grid for the input, 7x7 for the test
map = [['.' for i in range(map_size)] for j in range(map_size)]
def is_within_bounds(x, y):
    return x >= 0 and x < map_size and y >= 0 and y < map_size
directions = [[0,1], [0,-1], [1,0], [-1,0]]

# making a DFS to find a path to the end, we don't care about the shortest path, we just want a path 
def find_a_path(start = (0, 0), end = (map_size - 1, map_size - 1)):
        Q = []
        Q.append(start)
        visited = set()
        visited.add(start)
        while Q:
            current_pos = Q.pop()
            if current_pos == end:
                return True
            for dx, dy in directions:
                new_pos = (current_pos[0] + dx, current_pos[1] + dy)
                if not is_within_bounds(new_pos[0], new_pos[1]) or map[new_pos[0]][new_pos[1]] == '#' or new_pos in visited:
                    continue
                visited.add(new_pos)
                Q.append(new_pos)
        return False

answer_index = 0
for i in range(len(memory_space)):
    map[memory_space[i][0]][memory_space[i][1]] = '#'

    is_there_a_answer = find_a_path()
    # print("Is there a path? when adding ", memory_space[i], " : ", is_there_a_answer)  
    if not is_there_a_answer:
        answer_index = i
        break

# the answer is the first block to add that block the path
print(f"Answer: {memory_space[answer_index][0]},{memory_space[answer_index][1]}")
# we found 6,1 for the test18.txt file as expected