with open('input.txt', 'r') as f:
    lines = f.read().strip().split('\n')

nb_lines = len(lines)-1
len_line = len(lines[0])

signs = [line.strip() for line in lines[-1]]
signs = [s for s in signs if s != '']

separator_indexs = []
i = 0
while i < len_line:
    nb_empty = 0
    for j in range(nb_lines): # all number must be finished, because they can start on the right or the left and empty space inversely
        if lines[j][i] == ' ':
            nb_empty += 1
    if nb_empty == nb_lines:
        separator_indexs.append(i)
    i += 1

lines_splitted = []
for line in lines[:-1]: # exclude the last line that are the signs
    split_line = []
    prev_index = 0
    for index in separator_indexs:
        split_line.append(line[prev_index:index])
        prev_index = index + 1  
    split_line.append(line[prev_index:])  
    lines_splitted.append(split_line)


for line in lines_splitted:
    print(line)
print(signs)
"""
['123', '328', ' 51', '64 ']
[' 45', '64 ', '387', '23 ']
['  6', '98 ', '215', '314']
['*', '+', '*', '+']
"""
nb_col = len(lines_splitted[0])
total = 0
# for each column, the number are still using the same operators, but
# now there are gotten by looking vertically from the top to the bottom 
# 123
#  45
#   6
# gives 
# 1, 24, 356
# 64 
# 23 
# 314 
# gives 623, 431, 4

for col in range(nb_col):
    size_max = max(len(lines_splitted[row][col].strip()) for row in range(nb_lines))
    nums_array_ver = [lines_splitted[row][col] for row in range(nb_lines)]
    nums = []
    for i in range(size_max):
        num = ''
        for row in range(nb_lines):
            num += nums_array_ver[row][i]
        nums.append(num)

    """
    print('\n', nums_array_ver, '\n', nums)
    """

    if signs[col] == '+':
        result = sum(int(num) for num in nums)
        total += result
    elif signs[col] == '*':
        result = 1
        for num in nums:
            result *= int(num)
        total += result

print(total)
# found 3263827 as expected for the test.txt