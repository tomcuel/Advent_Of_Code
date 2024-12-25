# Opening the file in read mode
with open('input4.txt', 'r') as file:
    # extracting the matrix 
    matrix = file.read().strip().split('\n')

n = len(matrix)
m = len(matrix[0])
'''we need to to find something to look like this, and every way to turn it works 
M.S
.A.
M.S''' # counting as MSAMS
# there is 4 four possible ways to turn it (4 list containing the position of each letter)
# to make sure we don't count the same matrix twice, we full it with 1 at the postion of the A of the matrix we found
empty_matrix = [[0]*n for _ in range(m)]


# Define the 4 possible relative offsets for the "MAS" pattern starting from the "A"
directions = [
    # positions of M, S, S, M relative to A
    [(-1, -1), (-1, 1), (1, 1), (1, -1)],   # MSAMS
    [(-1, 1), (1, 1), (1, -1), (-1, -1)],  # MMASS
    [(1, 1), (1, -1), (-1, -1), (-1, 1)],  # SMASM
    [(1, -1), (-1, -1), (-1, 1), (1, 1)]   # SSAMM
]

answer = 0
# Iterate through the grid
for i in range(n):
    for j in range(m):
        if matrix[i][j] == 'A' and empty_matrix[i][j] == 0:
            for direction in directions:
                valid = True

                for dx, dy in direction:
                    nx, ny = i + dx, j + dy
                    if not (0 <= nx < n and 0 <= ny < m):  # Check bounds
                        valid = False
                        break
                    if (dx, dy) in [direction[0], direction[3]] and matrix[nx][ny] != 'M':  # Check 'M'
                        valid = False
                        break
                    if (dx, dy) in [direction[1], direction[2]] and matrix[nx][ny] != 'S':  # Check 'S'
                        valid = False
                        break

                if valid:
                    empty_matrix[i][j] = 1
                    answer += 1



       

print("Answer: ", answer)
# we found 9 for the test4.txt file as expected