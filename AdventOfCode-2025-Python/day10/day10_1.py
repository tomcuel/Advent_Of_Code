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

total = 0
from itertools import product
for light, button, joltage in zip(lights, buttons, joltages):
    L = len(light)
    M = len(button)

    # Build binary vectors for each button
    toggle = []
    for b in button:
        vec = [0]*L
        for idx in b:
            vec[idx] ^= 1
        toggle.append(vec)

    best = float('inf')
    # Enumerate all subsets of buttons: 2^M possibilities
    for mask in product([0,1], repeat=M):
        # Compute final lights
        out = [0]*L
        for i, press in enumerate(mask):
            if press:
                for j in range(L):
                    out[j] ^= toggle[i][j]

        if out == light:
            best = min(best, sum(mask))

    total += best

print(total)
# found 7 as expected for the test.txt