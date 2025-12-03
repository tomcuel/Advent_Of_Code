with open('input.txt', 'r') as f : 
    lines = f.read().strip().split('\n')


""" 
# first not clean code
total = 0 
for line in lines:
    idx = 0
    max = 0
    for i, v in enumerate(line[:-1]):
        if int(v) > max:
            max = int(v)
            idx = i

    idx2 = 0
    max = 0
    for j in range(idx+1, len(line)):
        v = line[j]
        if int(v) > max:
            max = int(v)
            idx2 = j

    number = int(line[idx]+line[idx2])
    total += number
    # print(idx, idx2, number)
"""


total = 0
for line in lines:
    digits = list(map(int, line))

    # index of maximum digit in the whole line (except last) so we have a 2 digits number 
    idx1 = max(range(len(digits) - 1), key=lambda i: digits[i])

    # index of maximum digit after idx1 for the units
    idx2 = max(range(idx1 + 1, len(digits)), key=lambda i: digits[i])

    number = int(f"{digits[idx1]}{digits[idx2]}")
    total += number
    # print(idx1, idx2, number)


print(total)
# found 357 as expected for the test.txt