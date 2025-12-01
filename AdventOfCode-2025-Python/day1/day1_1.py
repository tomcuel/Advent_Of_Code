with open('input.txt', 'r') as f : 
    lines = f.read().strip().split('\n')
rotations = []
for line in lines:
    rotations.append((line[0], int(line[1:])))

pos = 50 # starting position
count = 0 
for dir, clicks in rotations:
    if dir == 'L':
       pos = (pos - clicks) % 100
    if dir == 'R':
        pos = (pos + clicks) % 100
    if pos == 0:
        count += 1

print(count)
# found 3 as expected for the test.txt