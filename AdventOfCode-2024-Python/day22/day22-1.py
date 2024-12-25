# Opening the file
with open('input22.txt', 'r') as file:
    base_numbers = file.read().strip().split('\n')
base_numbers = [int(i) for i in base_numbers]

def mix(x,y) :
    return x ^ y 

def prune(x) :
    return x % 16777216

def get_next_number(x) : 
    x = prune(mix(x, x * 64))
    x = prune(mix(x, x // 32))
    x = prune(mix(x, x * 2048))
    return x

def get_number_after_index_reps(x, reps) :
    for _ in range(reps) :
        x = get_next_number(x)
    return x

answer = 0
for number in base_numbers :
    answer += get_number_after_index_reps(number, 2000)
# printing the answer
print("Answer: ", answer)
# we found 37327623 for the test22.txt file as expected