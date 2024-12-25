# import the regex module
import re

# Opening the file in read mode
with open('input3.txt', 'r') as file:
    # extracting the lines 
    lines = file.read().strip().split('\n')
# Join the lines to form a single string
line_only = "".join(lines)

# New pattern to capture mul(x, y), do() and don't()
pattern = r"mul\((\d+),(\d+)\)|(do\(\))|(don't\(\))"

# Apply regex search
matches = re.findall(pattern, line_only)
# Process matches
steps = []
for match in matches:
    if match[0]:  # Group for mul(x, y)
        steps.append((int(match[0]), int(match[1])))
    elif match[2]:  # Group for do()
        steps.append("do()")
    elif match[3]:  # Group for don't()
        steps.append("don't()")

answer  = 0
# We enable the calculation only when do() is called, and before don't() is called
calculation_enabled = True
for step in steps:
    if step == "do()":
        calculation_enabled = True
    elif step == "don't()":
        calculation_enabled = False
    else:
        if calculation_enabled:
            answer += step[0] * step[1]

# Print the result
print(answer)
# we find 48 for the test3_2.txt file as expected