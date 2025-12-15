with open('input.txt', 'r') as f:
    lines = f.read().strip().split('\n\n')

import re
P = re.compile(r"(\d+)")
from typing import List, Any

present_shapes = []
for line in lines[:-1]:
    splitline = line.strip().split(':\n')
    present = int(splitline[0])
    shape_lines = splitline[1].strip().split('\n')\
    
    # Ensure shape is rectangular
    width = max(len(row) for row in shape_lines)
    shape_matrix = []
    for row in shape_lines:
        shape_matrix.append([1 if c == '#' else 0 for c in row.ljust(width, '.')])
    present_shapes.append(shape_matrix)

regions = []
for line in lines[-1].strip().split('\n'):
    x = [int(i) for i in P.findall(line)]
    regions.append((x[0], x[1], x[2:])) # x[0] x x[1] areas to fit x[2] shape[0], ...

def print_mat(mat):
    for row in mat:
        print(''.join([str(e) for e in row]))

def rotate90(mat):
    return tuple([tuple(row) for row in zip(*mat[::-1])])

def rotate180(mat):
    return tuple([tuple(row[::-1]) for row in mat[::-1]])

def rotate270(mat):
    return rotate90(rotate180(mat))

def flip_horizontal(mat):
    return [row[::-1] for row in mat]

def flip_vertical(mat):
    return mat[::-1]

"""
mat = present_shapes[0]
print_mat(mat)
print_mat(rotate90(mat))
print_mat(rotate180(mat))
print_mat(rotate270(mat))
print_mat(flip_horizontal(mat))
print_mat(flip_vertical(mat))
"""

def all_rotations(mat):
    return set((tuple(tuple(i) for i in mat), rotate90(mat), rotate180(mat), rotate270(mat)))

# for m in all_rotations(mat):
#     print_mat(m)


stk_placed: List[List[tuple]] = []

def revert(space, present):
    """
    Revert last placement for present (pop from stk_placed)
    """
    places = stk_placed.pop()
    for x, y in places:
        if space[x][y] != present:
            # defensive: should never happen
            raise RuntimeError("Revert mismatch")
        space[x][y] = -1

def attempt_fit(x, y, space, present, presents):
    """
    Try to place one instance of 'present' at top-left position (x,y)
    'presents' is list of lists of orientations for each present type
    If successful, mark space with 'present' id and push placed coords to stk_placed, returning True, otherwise leave space unchanged and return False
    """
    pp = presents[present]
    H = len(space)
    W = len(space[0])
    for rot in pp:
        placed = []
        possible = True
        h = len(rot)
        w = len(rot[0])
        # try placing rot with its top-left mapped to (x,y)
        for i in range(h):
            for j in range(w):
                if not rot[i][j]:
                    continue
                a, b = x + i, y + j
                if a < 0 or b < 0 or a >= H or b >= W or space[a][b] != -1:
                    possible = False
                    break
                placed.append((a, b))
                space[a][b] = present
            if not possible:
                break
        if possible:
            stk_placed.append(placed)
            return True
        # undo partial placement for this orientation
        for a, b in placed:
            if space[a][b] == present:
                space[a][b] = -1
    return False

def next_empty(space, start_x, start_y):
    """
    Find next empty cell scanning row-major from (start_x, start_y)
    """
    H, W = len(space), len(space[0])
    # scan current row from start_y
    for y in range(start_y, W):
        if space[start_x][y] == -1:
            return start_x, y
    # scan subsequent rows
    for x in range(start_x + 1, H):
        for y in range(W):
            if space[x][y] == -1:
                return x, y
    return None

def helper(counts, space, presents) -> int:
    """
    Backtracking helper:
    counts: mutable list telling how many pieces of each present remain to place
    space: 2D grid with -1 = empty, otherwise present id
    returns 1 if it's possible to place all requested counts, 0 otherwise
    """
    # success if all counts zero
    if all(p == 0 for p in counts):
        return 1

    pos = next_empty(space, 0, 0)
    # if no empty but still counts: fail
    if pos is None:
        return 0

    # We will iterate through empty positions starting from pos
    cur_pos = pos
    while cur_pos is not None:
        x, y = cur_pos
        # try each present type at this position
        for p in range(len(counts)):
            if counts[p] == 0:
                continue
            if attempt_fit(x, y, space, p, presents):
                counts[p] -= 1
                if helper(counts, space, presents):
                    return 1
                # backtrack
                revert(space, p)
                counts[p] += 1
        # move to next empty cell after current y
        cur_pos = next_empty(space, x, y + 1)
    return 0

# too slow and impossible to run
def solve(present_shapes: List[List[List[int]]], regions: List[Any]) -> int: 
    """ 
    present_shapes: list of matrices (0/1) for each present type regions: list of tuples (width, height, counts_list)
    returns sum of successes (1 if region can be filled, 0 else) across all regions 
    """ 
    # prepare all orientations for each present 
    presents = [all_rotations(p) for p in present_shapes] 
    total = 0 
    for (width, height, counts) in regions: 
        # deep copy of counts, ensure length matches presents 
        counts_copy = list(counts) 
        if len(counts_copy) < len(presents): 
        # pad with zeros if counts missing for some shapes 
            counts_copy += [0] * (len(presents) - len(counts_copy)) 
        elif len(counts_copy) > len(presents): # more counts than shapes -> impossible 
            total += 0 
            continue 
    # create empty space: height rows, width columns 
    space = [[-1 for _ in range(width)] for __ in range(height)]
    stk_placed.clear() 
    ok = helper(counts_copy, space, presents)
    total += ok
    return total


def solve(present_shapes: List[List[List[int]]], regions: List[Any]) -> int:
    """
    regions: list of tuples (width, height, counts_list)
    returns sum of successes (1 if region can be filled, 0 else) across all regions
    just because it work
    """
    # precompute area of each present
    present_areas = [sum(sum(row) for row in p) for p in present_shapes]

    total = 0
    for (width, height, counts) in regions:
        # check if sum of areas <= region area
        area_needed = sum(a * b for a, b in zip(counts, present_areas))
        region_area = width * height
        total += int(area_needed <= region_area)
    return total

result = solve(present_shapes, regions)
print(result)





test = False
if test:
    # ======================
    # testing revert function
    # ======================
    def print_space(space):
        for row in space:
            print(" ".join(f"{c:2d}" for c in row))
        print()

    # 4x4 empty grid
    space = [[-1 for _ in range(4)] for __ in range(4)]

    print("Initial space:")
    print_space(space)

    # Fake present id
    present = 3

    # Simulate placing 3 blocks at (0,0), (0,1), (1,0)
    placed_cells = [(0, 0), (0, 1), (1, 0)]

    # Mark space and push stack entry
    for x, y in placed_cells:
        space[x][y] = present
    stk_placed.append(placed_cells)

    print("After placement:")
    print_space(space)

    # Now revert it:
    revert(space, present)

    print("After revert:")
    print_space(space)



    # ======================
    # testing to find if a piece fit
    # ======================
    space = [[-1]*4 for _ in range(4)]
    present_id = 0

    # Define a simple 2x2 present with only one orientation
    present_shape = [[1, 1],
                    [1, 0]]

    presents = [[present_shape]]  # list of orientations per present

    print("Initial space:")
    print_space(space)

    # Try placing at top-left corner
    success = attempt_fit(0, 0, space, present_id, presents)
    print("Placement success:", success)
    print_space(space)
    print("stk_placed:", stk_placed)

    # Try placing overlapping (should fail)
    success2 = attempt_fit(0, 1, space, present_id, presents)
    print("Placement success (overlap):", success2)
    print_space(space)
    print("stk_placed:", stk_placed)

    # Revert first placement
    revert(space, present_id)
    print("After revert:")
    print_space(space)
    print("stk_placed:", stk_placed)



    # ======================
    # attempt_fit, revert, next_empty, helper
    # ======================
    stk_placed = []

    # Define a 4x4 empty space
    space = [[-1]*4 for _ in range(4)]

    # Define 2 present types
    present0 = [[1, 1],
                [1, 0]]  # 2x2 L-shape
    present1 = [[1, 1, 1]]  # 1x3 horizontal
    present2 = [[1, 1],
                [1, 1],
                [1, 1]
                ]  # 1x3 horizontal

    # Each present only has one orientation for simplicity
    presents = [
        [present0],  # type 0
        [present1],   # type 1
        [present2]   # type 2
    ]

    # We want to place 1 of each
    counts = [1, 1, 1]

    print("Initial space:")
    print_space(space)

    # Attempt to place all using helper
    success = helper(counts, space, presents)

    print("Success:", success)
    print("Final space:")
    print_space(space)
    print("stk_placed:", stk_placed)