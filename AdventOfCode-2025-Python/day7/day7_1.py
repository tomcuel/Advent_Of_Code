with open('input.txt', 'r') as f:
    lines = f.read().strip().split('\n')

start = -1
for i in range(len(lines[0])):
    if lines[0][i] == 'S':
        start = i
        break

# go downward from start pos ('S') and replace the . by  |
# when encountering a ^ split around it so '.^.' becomes '|^|'
"""
lines[1] = lines[1][:start] + '|' + lines[1][start+1:]
for i in range(2, len(lines)):
    new_line = list(lines[i])
    for j in range(len(new_line)):
        if new_line[j] == '.':
            if lines[i-1][j] == '|':
                new_line[j] = '|'
        elif new_line[j] == '^':
            if lines[i-1][j] == '|':
                new_line[j] = '^'
                if j > 0 and new_line[j-1] == '.':
                    new_line[j-1] = '|'
                if j < len(new_line)-1 and new_line[j+1] == '.':
                    new_line[j+1] = '|'
    lines[i] = ''.join(new_line)

for line in lines:
    print(line)
"""

from collections import deque
total = 0
n, m = len(lines), len(lines[0])
col = {start}
for row in range(1, n):

    q = deque(col)
    col = set() # next row columns
    visited = set() # avoid double counting
    while q:
        c = q.popleft()
        if c in visited:
            continue
        visited.add(c)
        if not (0 <= c < m):
            continue

        if lines[row][c] == '^':
            total += 1 # a beam split, it stops here and spawns left/right
            if c > 0 and c-1 not in visited:
                q.append(c-1)
            if c < len(lines[0])-1 and c+1 not in visited:
                q.append(c+1)
        else: # continues downward
            col.add(c)

print(total)
# found 21 as expected for the test.txt