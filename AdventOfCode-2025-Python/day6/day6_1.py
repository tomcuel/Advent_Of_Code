with open('input.txt', 'r') as f:
    lines = f.read().strip().split('\n')

numbers = [line.strip().split() for line in lines[:-1]]
rotated_numbers = list(zip(*numbers[::-1]))
reversed_rotated_numbers = [list(line)[::-1] for line in rotated_numbers]
signs = [line.strip() for line in lines[-1]]
signs = [s for s in signs if s != '']

total = 0
for nums, sign in zip(reversed_rotated_numbers, signs):
    if sign == '+':
        result = sum(int(num) for num in nums)
        total += result
    elif sign == '*':
        result = 1
        for num in nums:
            result *= int(num)
        total += result

print(total)
# found 4277556 as expected for the test.txt