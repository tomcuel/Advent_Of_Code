# Opening the file in read mode
with open('input1.txt', 'r') as file:
    # extracting the lines 
    lines = file.read().strip().split('\n')

# gettig the pairs of numbers
pairs = [[int(num) for num in lines[i].split('   ')] for i in range(len(lines))]
left_list = [pairs[i][0] for i in range(len(pairs))]
right_list = [pairs[i][1] for i in range(len(pairs))]

# each number on the left list is represented n times on the right list 
number_of_occurrences = [[left_list[i], right_list.count(left_list[i])] for i in range(len(left_list))]

answer = 0
for i in range(len(number_of_occurrences)):
    answer +=  number_of_occurrences[i][0] * number_of_occurrences[i][1]

print("Answer: ", answer)
# we find 31 for the test1.txt file as expected