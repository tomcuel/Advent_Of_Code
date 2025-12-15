with open('input.txt', 'r') as f:
    lines = f.read().strip().split('\n')
lines = [tuple(map(int, line.split(','))) for line in lines]
coords = [(x,y) for x,y in lines]
n = len(coords)

# for each tile couple, calculate the rectangle area associated 
# x1, y1 and x2, y2 --> abs(x1-x2)+1 * abs(y1-y2)+1 
# need to suppress areas where there is another point in there strictly, since points represented area boundaries 
from itertools import combinations

ans = 0
for a, b in combinations(coords, 2):
    xmin, xmax = min(a[0], b[0]), max(a[0], b[0])
    ymin, ymax = min(a[1], b[1]), max(a[1], b[1])
    area = (xmax - xmin + 1) * (ymax - ymin + 1)

    if area <= ans:
        continue

    possible = True
    for a in range(n):
        b = (a + 1) % n
        x1, y1 = coords[a]
        x2, y2 = coords[b]

        if not (
            (y1 >= ymax and y2 >= ymax)
            or (y1 <= ymin and y2 <= ymin)
            or (x1 >= xmax and x2 >= xmax)
            or (x1 <= xmin and x2 <= xmin)
        ):
            possible = False
            break

    if possible:
        ans = area

print(ans)
# found 24 as expected for the test.txt