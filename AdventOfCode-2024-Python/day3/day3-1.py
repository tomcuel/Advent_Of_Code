# import the regex module
import re

# Opening the file in read mode
with open('input3.txt', 'r') as file:
    # extracting the lines 
    lines = file.read().strip().split('\n')
# Join the lines to form a single string
line_only = "".join(lines)

# Regex pattern to match numbers between "mul(" and ")"
pattern = r"mul\((\d+),(\d+)\)"

# Find all matches
matches = re.findall(pattern, line_only)

# Flatten the results to extract valid pairs
result = 0
for match in matches:
    result += int(match[0]) * int(match[1])

print("Answer: ", result)
# we find 161 for the test3_1.txt file as expected