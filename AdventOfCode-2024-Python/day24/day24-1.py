# Opening the file
with open('input24.txt', 'r') as file:
    file = file.read().strip().split('\n\n')
first_part = file[0].split('\n')
second_part = file[1].split('\n')

# dictionnary to store the values of the different gates
values = {}
for line in first_part:
    key, value = line.split(':')
    values[key] = int(value)

# dictonnary to store the different actions between gates to calculate others gates
actions = {}
for line in second_part:
    operation, key = line.split(' -> ')
    actions[key] = operation.split(' ')

while actions : 
    for key, operation in actions.items():
        if operation[0] in values and operation[2] in values : 
            if operation[1] == 'XOR' : 
                values[key] = values[operation[0]] ^ values[operation[2]]
            elif operation[1] == 'AND' :
                values[key] = values[operation[0]] & values[operation[2]]
            elif operation[1] == 'OR' :
                values[key] = values[operation[0]] | values[operation[2]]
            del actions[key]
            break
'''
We found this as expected for the first_test24.txt file
{'x00': 1, 'x01': 1, 'x02': 1, 'y00': 0, 'y01': 1, 'y02': 0, 'z00': 0, 'z01': 0, 'z02': 1}
'''
# sorting the values by key alphabetically
values = dict(sorted(values.items()))
# now we only want the values of the gates that begins with 'z' and assembling them to make a binary number
binary = ''
for key, value in values.items():
    if key[0] == 'z':
        binary = str(value) + binary
print("Binary:", binary)
# converting the binary number to decimal
decimal = int(binary, 2)
print("Answer:", decimal) 
# we found 2024 as expected for the test24.txt file