from sympy import symbols, Eq, solve # --> used for part 2 solving

# Opening the file in read mode
with open('test13.txt', 'r') as file:
    input = file.read().strip().split('\n\n')

commands = []
for i,command in enumerate(input):
    command = command.split('\n')
    buttonA = command[0].split(' ')
    buttonB = command[1].split(' ')
    prize = command[2].split(' ')
    Xa = int(buttonA[2][2:-1])
    Ya = int(buttonA[3][2:])
    Xb = int(buttonB[2][2:-1])
    Yb = int(buttonB[3][2:])
    Xpri = int(prize[1][2:-1]) + 10000000000000
    Ypri = int(prize[2][2:]) + 10000000000000
    commands.append((Xa, Ya, Xb, Yb, Xpri, Ypri))

def get_token(command):
    Xa, Ya, Xb, Yb, Xpri, Ypri = command
    total = 0

    # Define symbols for A and B
    A, B = symbols('A B', integer=True)
    
    # Equations and system solving
    eq1 = Eq(Xa * A + Xb * B, Xpri)
    eq2 = Eq(Ya * A + Yb * B, Ypri)
    solutions = solve((eq1, eq2), (A, B))
    
    # Ensure the solutions are integers and non-negative
    if solutions and all(value >= 0 for value in solutions.values()):
        A_val, B_val = int(solutions[A]), int(solutions[B])
        cost = 3 * A_val + B_val
        total += cost
    
    return total


answer = 0
for command in commands:
    answer += get_token(command)
# printing the answer
print("The answer is:", answer)
# We found only the 2 and 4 working for the test13.txt file as expected