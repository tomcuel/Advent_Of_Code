from itertools import permutations, product

# Opening the file
with open('test21.txt', 'r') as file:
    instructions = file.read().strip().split('\n')
nb_instructions = len(instructions)

'''
the numeric keypad looks like this:
7 8 9
4 5 6
1 2 3
  0 A
'''
numeric_keypad = {
    "7": (0, 0),
    "8": (0, 1),
    "9": (0, 2),
    "4": (1, 0),
    "5": (1, 1),
    "6": (1, 2),
    "1": (2, 0),
    "2": (2, 1),
    "3": (2, 2),
    "0": (3, 1),
    "A": (3, 2)
}
'''
the direction keypad looks like this:
  ^ A
< v >
'''
direction_keypad = {
    "^": (0, 1),
    "A": (0, 2),
    "<": (1, 0),
    "v": (1, 1),
    ">": (1, 2)
}
# the direction keypad looks like this for each directionnal key
key_directions = {
    ">": (0, 1),
    "v": (1, 0),
    "<": (0, -1),
    "^": (-1, 0)
}
'''
test file : 029A
- "<" to move the arm from A (its initial position) to 0.
- "A" to push the 0 button.
- "^A" to move the arm to the 2 button and push it.
- ">^^A" to move the arm to the 9 button and push it.
- "vvvA" to move the arm to the A button and push it.
Those are the three shortest instructions to do it : 
<A^A>^^AvvvA
<A^A^>^AvvvA
<A^A^^>AvvvA
--> that's for the human way
'''

def ways(code, keypad_type):

    parts = [] # number of ways to press the code on the keypad
    current_position = keypad_type["A"] # begin at A

    for case in code : 
        # get the position of the key
        next_position = keypad_type[case]
        # get the number of moves to do in both directions
        di = next_position[0] - current_position[0]
        dj = next_position[1] - current_position[1]
        moves = ""
        if di > 0:
            moves += "v" * di
        elif di < 0:
            moves += "^" * -di
        if dj > 0:
            moves += ">" * dj
        elif dj < 0:
            moves += "<" * -dj

        # we can make a permutation of the moves to which we add the push instruction "A"
        # If moves = "vv>>", possible permutations include "vv>>A", "v>v>A", "v>>vA", and so on.
        combo_list = list(set(["".join(x) + "A" for x in permutations(moves)]))
        # then we need to get only the one that are valid
        combos = []
        # Simulate the movement, starting from current_position.
        # For each move (c), calculate the next position using key_directions to check if it is valid (exist in the keypad).
        for combo in combo_list:
            good = True
            ci, cj = current_position
            for c in combo[:-1]:
                di, dj = key_directions[c]
                ci, cj = ci + di, cj + dj
                if not (ci,cj) in keypad_type.values():
                    good = False
                    break
            if good:
                combos.append(combo)

        parts.append(combos)
        current_position = next_position
    
    return ["".join(x) for x in product(*parts)]

def get_shortest_way(code) : 
    # numeric keypad for men 
    numeric_ways = ways(code, numeric_keypad)
    # but for each human way, a robot needs to operate the direction keypad
    robot_1_ways = []
    for way in numeric_ways : 
        robot_1_ways.extend(ways(way, direction_keypad))
    # then another robot needs to operate the numeric keypad for each robot 1 way
    robot_2_ways = []
    for way in robot_1_ways : 
        robot_2_ways.extend(ways(way, direction_keypad))
    
    # then we need to get the shortest way to do it
    return min([len(x) for x in robot_2_ways])


answer = 0
for line in instructions:
    print(get_shortest_way(line), int(line[:-1]))
    answer += get_shortest_way(line) * int(line[:-1])

print(answer)
# we got the right answer for the test file : 126 384 
'''
with :
- 029A with 68 presses
- 980A with 60 presses
- 179A with 68 presses
- 456A with 64 presses
- 379A with 64 presses
'''