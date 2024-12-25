import re
from itertools import product

# Opening the file in read mode
with open('input7.txt', 'r') as file:
    # splitting the pages inequalities and the upgrade lectures
    lines = file.read().strip().split('\n')

# a line is "number: number number … … number"
# we need to extract the numbers with a regular expression and convert them to integers
regex_expression = r'\d+'
lines = [list(map(int, re.findall(regex_expression, line))) for line in lines]

# we need to find the lines where line[0] is an operation of the numbers from line[1] to line[len(line)-1] 
# the operation can be either + or *, we need to test all the possibilities until we find one that matches the number_to_match, or return False
def can_operate(number_to_match, numbers, combo) : 
    number_created = numbers[0]
    for i in range(len(numbers)-1) : 
        if combo[i] == "+" : 
            number_created += numbers[i+1]
        else : 
            number_created *= numbers[i+1]
    return number_created == number_to_match
    

# we sum the results of the lines operation
answer = 0 
for line in lines : 
    # we test every combination of + and * for the numbers in the line
    for combo in product("+*", repeat=len(line)-1) : 
        # if we found one that matches the number_to_match, we add the number to the answer 
        # and break the loop not to repeat a test to see if another combo can do it
        if can_operate(line[0], line[1:], combo):
            answer += line[0]
            break
    

# print the answer
print("Answer:", answer)
# we found 379 for the test7.txt file as expected