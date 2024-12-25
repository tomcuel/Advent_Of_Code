# Opening the file in read mode
with open('input1.txt', 'r') as file:
    # extracting the lines 
    lines = file.read().strip().split('\n')

# gettig the pairs of numbers
pairs = [[int(num) for num in lines[i].split('   ')] for i in range(len(lines))]
left_list = sorted([pairs[i][0] for i in range(len(pairs))])
right_list = sorted([pairs[i][1] for i in range(len(pairs))])

# calculating the sum of differences between the pairs
sum = 0
for i in range(len(pairs)):
    sum += abs(left_list[i] - right_list[i])

print("Answer: ", sum)
# we find 11 for the test1.txt file as expected