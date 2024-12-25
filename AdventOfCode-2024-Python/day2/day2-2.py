# Opening the file in read mode
with open('input2.txt', 'r') as file:
    # extracting the lines 
    lines = file.read().strip().split('\n')

# getting the matrix array
reports = [[int(num) for num in lines[i].split(' ')] for i in range(len(lines))]
n = len(reports)

number_of_safe_lines = 0
# comparing for each line if there is a 1-3 number decrease or increase at each step in the line 
# but there need to be only increase of decrease, not both
# now we can remove one elements and see if it solve the problem of a line validity 

def is_line_safe(line) : 
    # Calculate the differences between consecutive elements
    differences = [line[i+1] - line[i] for i in range(len(line) - 1)]

    # Check if all differences are positive or all negative
    is_increasing = all(diff > 0 for diff in differences)
    is_decreasing = all(diff < 0 for diff in differences)

    # Check if all differences are within 1-3 in absolute value
    valid_magnitude = all(1 <= abs(diff) <= 3 for diff in differences)
    
    # Line is safe if it's consistently increasing or decreasing with valid differences
    return valid_magnitude and (is_increasing or is_decreasing)

for line in reports : 
    # Try removing one element at a time and check if the line becomes safe
    for i in range(len(line)):
        modified_line = line[:i] + line[i+1:]  # Remove the i-th element
        if is_line_safe(modified_line):
            number_of_safe_lines += 1
            break
    
print("Answer: ", number_of_safe_lines)
# we find 4 for the test2.txt file as expected