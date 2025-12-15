with open('input.txt', 'r') as f:
    lines = f.read().strip().split('\n')

lines = [line.strip().split(' ') for line in lines]

lights_temp = [line[0][1:-1] for line in lines]
lights = [[i for i in range(len(line)) if line[i] == '#'] for line in lights_temp]
lights = [[1 if c == '#' else 0 for c in line] for line in lights_temp]

buttons_temp = [list(e[1:-1] for e in line[1:-1]) for line in lines]
buttons = []
for line in buttons_temp:
    button = []
    for elem in line:
        elem = list(int(e) for e in elem.strip().split(','))
        button.append(elem)
    buttons.append(button)

joltages = [list(int(e) for e in line[-1][1:-1].strip().split(',')) for line in lines]

from itertools import product
from collections import deque

def solve_p2_line(button, joltage):
    J = len(joltage)
    M = len(button)

    # Build binary vectors for each button
    eff = []
    for b in button:
        vec = [0]*J
        for idx in b:
            vec[idx] ^= 1
        eff.append(vec)

    # BFS state: tuple(counter values)
    start = tuple([0]*J)
    joltage = tuple(joltage)
    if start == joltage:
        return 0

    q = deque([(start, 0)])
    seen = {start}
    while q:
        state, steps = q.popleft()

        for i in range(M):
            new_state = list(state)

            # Add this button's effects, that change the state of the pushed button
            for k in range(J):
                new_state[k] += eff[i][k]
                if new_state[k] > joltage[k]:
                    break
            else:
                new_state = tuple(new_state)
                if new_state == joltage:
                    return steps + 1
                if new_state not in seen:
                    seen.add(new_state)
                    q.append((new_state, steps+1))

    return float('inf') # error


total = 0
for _, button, joltage in zip(lights, buttons, joltages):
    total += solve_p2_line(button, joltage)
print(total)
# found 33 as expected for the test.txt (too slow for the long input.txt file)