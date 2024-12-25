# Opening the file in read mode
with open('input4.txt', 'r') as file:
    # extracting the matrix 
    matrix = file.read().strip().split('\n')

answer = 0
n = len(matrix)
m = len(matrix[0])
directions = [
    (0, 1),   # Right
    (0, -1),  # Left
    (-1, 0),  # Up
    (1, 0),   # Down
    (-1, 1),  # Diagonal up-right
    (-1, -1), # Diagonal up-left
    (1, 1),   # Diagonal down-right
    (1, -1)   # Diagonal down-left
]
# iterate over the matrix
# each time we find a X at the position (i,j) we look in all directions (up, down, left, right, diagonals (4 of them))
# and see if a XMAS appears in this order
for i in range(n):
    for j in range(n):
        if matrix[i][j] == 'X':
            # for each direction, we check if the next 3 characters are M, A, S and if we're within the bounds of the matrix
            for dx, dy in directions:
                if all(0 <= i + k * dx < n and 0 <= j + k * dy < m and matrix[i + k * dx][j + k * dy] == letter for k, letter in enumerate("MAS", start=1)):
                    answer += 1
            

print("Answer: ", answer)
# we found 18 for the test4.txt file as expected