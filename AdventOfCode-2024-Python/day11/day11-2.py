# Opening the file in read mode
with open('input11.txt', 'r') as file:
    line = file.read().strip().split(' ')
line_nums = [int(num) for num in line]


# convert the inputs to a string for memoization
def inputs_to_str(nums, blinks):
    return "".join(str(num) + "_" for num in nums) + f"+{blinks}"

# memoization for the rules and calculations
rule_memo = {}
calc_memo = {}

# apply the same rules as part 1
def apply_rules(num):
    if num not in rule_memo:
        num_str = str(num)
        num_str_len = len(num_str)
        if num == 0:
            rule_memo[num] = [1]
        elif num_str_len % 2 == 0:
            rule_memo[num] = [int(num_str[num_str_len//2:]), int(num_str[:num_str_len//2])]
        else:
            rule_memo[num]  = [num * 2024]
    return rule_memo[num]

# for calculating the stones at each step by looking what each stone give us for the following steps
def calc_stones(nums, blinks):
    input_str = inputs_to_str(nums, blinks)
    if input_str not in calc_memo:
        if blinks == 0:
            calc_memo[input_str] = len(nums)
        else : 
            calc_memo[input_str] = sum(calc_stones(apply_rules(num), blinks - 1) for num in nums)
    return calc_memo[input_str]


# print the answer
print("Answer:", calc_stones(line_nums, 75))