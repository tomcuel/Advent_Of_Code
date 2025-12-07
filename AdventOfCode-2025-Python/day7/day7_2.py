with open('input.txt', 'r') as f:
    lines = f.read().strip().split('\n')

start = -1
for i in range(len(lines[0])):
    if lines[0][i] == 'S':
        start = i
        break

from collections import deque
n, m = len(lines), len(lines[0])
col_counts = {start: 1}
for row in range(1, n):
    if not col_counts:
        break

    q = deque(col_counts.items())
    new_col_counts = {}
    while q:
        col, count = q.popleft()
        if count == 0:
            continue
        if not (0 <= col < m):
            continue

        if lines[row][col] == '^':
            # a beam split, it stops here and spawns left/right
            if col > 0:
                q.append((col-1, count))
            if col < len(lines[0])-1:
                q.append((col+1, count))
        else: # continues downward, update new_col_counts to know the number of paths reaching this column
            new_col_counts[col] = new_col_counts.get(col, 0) + count

    col_counts = new_col_counts # update for next row

print(sum(col_counts.values()))
# found 40 as expected for the test.txt