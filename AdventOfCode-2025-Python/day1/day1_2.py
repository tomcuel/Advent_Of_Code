with open('input.txt', 'r') as f : 
    lines = f.read().strip().split('\n')
rotations = [(line[0], int(line[1:])) for line in lines]


pos = 50
total = 0
for direction, clicks in rotations:
    
    if direction == 'L':
        # first is the number of minimal clicks in this direction to reach a 0
        first = pos
        if first == 0:
            first = 100
        pos = (pos - clicks) % 100

    if direction == 'R':
        first = 100 - pos
        if first == 0:
            first = 100
        pos = (pos + clicks) % 100


    if clicks < first: # avoid adding the 1 when there is no pass through 0
        continue
    total += 1 + (clicks - first) // 100


print(total)
# found 6 as expected for the test.txt