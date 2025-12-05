with open('input.txt', 'r') as f:
    block1, block2 = f.read().strip().split('\n\n')

# Parse fresh intervals
fresh = []
for line in block1.splitlines():
    a, b = map(int, line.split('-'))
    fresh.append((a, b))

# Parse available integers
available = [int(x) for x in block2.splitlines()]

# Count the number of different fresh numbers given the ranges, available not used there 
"""
unique_number = set()
for fr_start, fr_end in fresh:
    for i in range(fr_start, fr_end+1):
        if i not in unique_number:
            unique_number.add(i)
# too slow (set) need to merge ranges
"""
# Merge overlapping intervals ONLY
events = []  # (position, is_start = 1 if start, = -1 if end)
for a, b in fresh:
    events.append((a, 1))      # coverage starts at a
    events.append((b + 1, -1)) # coverage ends just before b

# Sort by position; if same position, start events (+1) before end events (-1)
events.sort()
active = 0
total = 0
prev_pos = events[0][0]
i = 0
n = len(events)
while i < n:
    pos = events[i][0]
    # Accumulate coverage from prev_pos up to pos (not including pos) since they are sorted
    if pos > prev_pos and active > 0:
        total += pos - prev_pos
    # Process all events at this position
    while i < n and events[i][0] == pos:
        active += events[i][1] # How many intervals currently cover the number line
        i += 1
    prev_pos = pos

print(total)
# found 14 as expected for the test.txt