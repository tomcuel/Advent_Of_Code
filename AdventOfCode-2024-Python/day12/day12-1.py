from collections import defaultdict, deque

# Opening the file in read mode
with open('input12.txt', 'r') as file:
    lines = file.read().strip().split('\n')
lines = [[c for c in line] for line in lines]
# function to check if the position is within the bounds of the grid
n = len(lines)
m = len(lines[0])
def is_within_bounds(x, y):
    return x >= 0 and x < n and y >= 0 and y < m

directions = [(1, 0), (-1, 0), (0, 1), (0, -1)]

# getting the letters and their positions
letters_positions = defaultdict(list)
for i in range(len(lines)):
    for j in range(len(lines[i])):
            letters_positions[lines[i][j]].append((i, j))

# we have all the point linked to the letters, now we need to parse them into blocks and represent them as below
'''
letter_blocks["O1"] = [(0, 0), (0, 1), (0, 2), (0, 3), (0, 4), (1, 0), (1, 2), (1, 4), (2, 0), (2, 1), (2, 2), (2, 3), (2, 4), (3, 0), (3, 2), (3, 4), (4, 0), (4, 1), (4, 2), (4, 3), (4, 4)]
letter_blocks["X1"] = [(1, 1)]
letter_blocks["X2"] = [(3, 1)]
letter_blocks["X3"] = [(1, 3)]
letter_blocks["X4"] = [(3, 3)]
'''
# getting the blocks for the letters
letter_blocks = defaultdict(list)

# Function to perform BFS to find connected components
def bfs_get_block(x, y, letter, visited):
    queue = deque([(x, y)])
    block = []
    visited.add((x, y))

    while queue:
        cx, cy = queue.popleft()
        block.append((cx, cy))

        for dx, dy in directions:
            nx, ny = cx + dx, cy + dy
            if is_within_bounds(nx, ny) and (nx, ny) not in visited and lines[nx][ny] == letter:
                visited.add((nx, ny))
                queue.append((nx, ny))
    return block

# Find all blocks for each letter
visited = set()
for letter in letters_positions:
    block_id = 1
    for x, y in letters_positions[letter]:
        if (x, y) not in visited:
            block = bfs_get_block(x, y, letter, visited)
            letter_blocks[f"{letter}{block_id}"] = block
            block_id += 1

# getting the answer
answer = 0
for letters in letter_blocks:
    letter = letters[0]

    # now we need to count the number of time the letter occurs in the grid
    number_of_occurrences = len(letter_blocks[letters])
    # we need to count the perimeter that the letters blocks are defining in the grid
    # a perimeter is all the number of edges of the area containing the letters
    # an edge is a border or a neighbour point that is not the same letter
    perimeter = 0
    for x,y in letter_blocks[letters]:
        for dx, dy in directions:
            new_x, new_y = x + dx, y + dy
            if not is_within_bounds(new_x, new_y) or lines[new_x][new_y] != letter:
                perimeter += 1
    answer += number_of_occurrences * perimeter


# print the answer
print("Answer:", answer)
# we found 140 for the test12_1.txt file as expected
# we found 772 for the test12_2.txt file as expected
# we found 1930 for the test12.txt file as expected