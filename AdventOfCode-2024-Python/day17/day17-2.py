from multiprocessing import Pool

# Opening the file
with open('test17_2.txt', 'r') as file:
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

def test_A(A, program, B, C):
    # then we do as in the part 1 of the puzzle to process the program
    instruction_pointer = 0
    output = []

    # Simulation loop
    while instruction_pointer < len(program):
        opcode = program[instruction_pointer]
        operand = program[instruction_pointer + 1]
        instruction_pointer += 2
        
        pre_comp_combo = [0,1,2,3,A,B,C]
        if opcode == 0:  # adv
            A >>= pre_comp_combo[operand]
        elif opcode == 1:  # bxl
            B ^= operand
        elif opcode == 2:  # bst
            B = pre_comp_combo[operand] % 8
        elif opcode == 3:  # jnz
            if A != 0:
                instruction_pointer = operand
        elif opcode == 4:  # bxc
            B ^= C
        elif opcode == 5:  # out
            output.append(pre_comp_combo[operand] & 7)
        elif opcode == 6:  # bdv
            B = A >> pre_comp_combo[operand]
        elif opcode == 7:  # cdv
            C = A >> pre_comp_combo[operand]

        if len(output) > len(program) or output != program[:len(output)] : # output must match the program
            break

    return output == program


A_final = 0
start = 0
end = 1000000000000
step = 1
for i in range (start, end, step) :
    if test_A(i, Program, Register_B, Register_C) :
        A_final = i
        break
print("Answer : ", A_final)
# we found 117440 for the test17_2.txt file as expected
# however, after making the code run faster, I didn't find the pattern for the input17.txt file
# and the loop will not work because the number we're searching for is too big
# I didn't find a way so this puzzle will not be puzzle 