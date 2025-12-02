with open('input.txt', 'r') as f : 
    lines = f.read().strip().split(',')
ids = []
for line in lines:
    split = line.strip().split('-')
    ids.append((int(split[0]), int(split[1])))

print(ids)
    
def has_repeating_pattern(id):
    middle = len(id) // 2 
    return id[0:middle] == id[middle:]

total_ids = 0 
for start, end in ids:
    for id in range(start, end+1):
        if has_repeating_pattern(str(id)):
            total_ids += id

print(total_ids)
# found 1227775554 as expected for the test.txt