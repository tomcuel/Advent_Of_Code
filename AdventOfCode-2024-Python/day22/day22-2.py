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


def get_profits_for_a_number(x) :
    # now we only need the last digit of the sequence in the part 2
    # each buyer (line number) generates a sequence of 2000 numbers
    sequence = [x % 10]
    profit = {}
    new_x = x
    for i in range (2000) : 
        new_x = get_next_number(new_x)
        sequence.append(new_x % 10)

        # the monkey can only make the change after 4 exchanges
        if i >= 5 :
            changes = [sequence[j] - sequence[j-1] for j in range(i-2, i+2)]
            if not tuple(changes) in profit :
                profit[tuple(changes)] = new_x % 10
    
    return profit
# list of all the profit made by each buyer at every selling point
profits = [get_profits_for_a_number(number) for number in base_numbers]


def get_total_profit_of_sequence(sequence) : 
    total_profit = 0
    for profit in profits :
        if sequence in profit :
            total_profit += profit[sequence]
    return total_profit


sequences = set()
for profit in profits :
    sequences = sequences.union(profit.keys())
answer = 0
for sequence in sequences :
    answer = max(answer, get_total_profit_of_sequence(sequence))
# printing the answer
print("Answer : ", answer)