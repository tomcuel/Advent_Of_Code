with open('input.txt', 'r') as f:
    lines = f.read().strip().split('\n')

lines = [line.strip().split(' ') for line in lines]

lights_temp = [line[0][1:-1] for line in lines]
lights = [[i for i in range(len(line)) if line[i] == '#'] for line in lights_temp]
lights = [list((c == '#') for c in line) for line in lights_temp]

buttons_temp = [list(e[1:-1] for e in line[1:-1]) for line in lines]
buttons = []
for line in buttons_temp:
    button = []
    for elem in line:
        elem = list(int(e) for e in elem.strip().split(','))
        button.append(elem)
    buttons.append(button)

joltages = [list(int(e) for e in line[-1][1:-1].strip().split(',')) for line in lines]

import numpy as np
import scipy.optimize

total_presses = 0
for button, joltage in zip(buttons, joltages):
    n = len(joltage)      
    m = len(button)    

    # Build A_eq matrix: rows = counters, columns = buttons
    A_eq = np.zeros((n, m), dtype=int)
    for j, idxs in enumerate(button):
        for k in idxs:
            A_eq[k, j] = 1

    # Objective: minimize total presses
    c = np.ones(m, dtype=float)
    # RHS = target joltage levels
    b_eq = np.array(joltage, dtype=int)
    # Buttons can be pressed any non-negative number of times
    bounds = [(0, None)] * m

    # Solve LP (continuous relaxation)
    r = scipy.optimize.linprog(
        c=c,
        A_eq=A_eq,
        b_eq=b_eq,
        bounds=bounds,
        integrality=np.ones(m, dtype=int), # integer support and return type
        method="highs"
    )
    total_presses += int(r.fun)

print(total_presses)
# found 33 as expected for the test.txt