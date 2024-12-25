# Opening the file
with open('input17.txt', 'r') as file:
    file = file.read().strip().split('\n')
for i in range(3) : 
    file[i] = file[i].split(': ')
file[4] = file[4].split(': ')
file[4] = file[4][1].split(',')
Register_A = int(file[0][1])
Register_B = int(file[1][1])
Register_C = int(file[2][1])
Program = [int(file[4][i]) for i in range(len(file[4]))]

'''
Implement each opcode's logic as described:
- adv (0): Perform integer division on A based on 2^operand
- bxl (1): Bitwise XOR B with the operand.
- bst (2): Modulo the operand by 8 and store in B.
- jnz (3): If A != 0, jump to the operand position. 
- bxc (4): XOR B and C.
- out (5): Output the operand modulo 8.
- bdv (6): Division on A stored in C
- cdv (7): Division on A stored in C
'''

# Initialize instruction pointer and output
instruction_pointer = 0
output = []

# Function to evaluate combo operands
def eval_combo(operand):
    if operand in range(4):  # 0-3
        return operand
    elif operand == 4:
        return Register_A
    elif operand == 5:
        return Register_B
    elif operand == 6:
        return Register_C

# Simulation loop
while instruction_pointer < len(Program):
    # getting the opcode and operand
    opcode = Program[instruction_pointer]
    operand = Program[instruction_pointer + 1]
    instruction_pointer += 2

    if opcode == 0:  # adv
        Register_A //= 2 ** eval_combo(operand)
    elif opcode == 1:  # bxl
        Register_B ^= operand
    elif opcode == 2:  # bst
        Register_B = eval_combo(operand) % 8
    elif opcode == 3:  # jnz
        if Register_A != 0:
            instruction_pointer = operand # its not getting +2 for the loop increment
    elif opcode == 4:  # bxc
        Register_B ^= Register_C
    elif opcode == 5:  # out
        output.append(eval_combo(operand) % 8)
    elif opcode == 6:  # bdv
        Register_B = Register_A // (2 ** eval_combo(operand))
    elif opcode == 7:  # cdv
        Register_C = Register_A // (2 ** eval_combo(operand))

# Join outputs
result = ",".join(map(str, output))
print(result)
# we found 4,6,3,5,6,3,5,2,1,0 for the test17_1.txt file as expected