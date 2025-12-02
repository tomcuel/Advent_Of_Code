with open('input.txt', 'r') as f : 
    lines = f.read().strip().split(',')
ids = []
for line in lines:
    split = line.strip().split('-')
    ids.append((int(split[0]), int(split[1])))

print(ids)
    
def has_repeating_pattern(id):
    n = len(id)
    for i in range(1, n//2 +1): # a repeating pattern cannot exceed the middle 
        if n % i == 0: # the must be able to be repeated in the number
            pattern = id[:i] 
            if pattern * (n // i) == id: # the pattern repeated the number of times should correspond to the id
                return True

total_ids = 0 
for start, end in ids:
    for id in range(start, end+1):
        if has_repeating_pattern(str(id)):
            total_ids += id

print(total_ids)
# found 4174379265 as expected for the test.txt