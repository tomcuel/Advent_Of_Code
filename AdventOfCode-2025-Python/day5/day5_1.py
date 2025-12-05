with open('input.txt', 'r') as f:
    block1, block2 = f.read().strip().split('\n\n')

# Parse fresh intervals
fresh = []
for line in block1.splitlines():
    a, b = map(int, line.split('-'))
    fresh.append((a, b))

# Parse available integers
available = [int(x) for x in block2.splitlines()]

# Count how many available numbers fall within any fresh interval
total = 0
for av in available:
    for start, end in fresh:
        if start <= av <= end:
            total += 1
            break  # avoid counting twice

print(total)
# found 3 as expected for the test.txt