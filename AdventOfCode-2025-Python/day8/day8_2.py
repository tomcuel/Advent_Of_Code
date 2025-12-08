with open('input.txt', 'r') as f:
    lines = f.read().strip().split('\n')

lines = [line.strip().split(',') for line in lines]
coords = [[int(i) for i in line] for line in lines]
# for line in coords:
#     print(line)
n = len(coords)

from math import sqrt
def get_distance(c1, c2):
    return sqrt(sum((c1[i] - c2[i]) ** 2 for i in range(3)))

# make all the connections given the number of connections
# then link the junction boxes to know the sizes of the circuit 
# make a dict by lenght of the circuit (= number of points in it that are linked together)

from itertools import combinations
from collections import defaultdict

# Compute all pairwise distances
pairs = [] 
for i, j in combinations(range(n), 2):
    d = get_distance(coords[i], coords[j])
    pairs.append((d, i, j))
pairs.sort(key=lambda x: x[0])

# Select the N shortest connections
number_connections = 1000 # 10 for the test
chosen_connections = pairs[:number_connections]
# for d, a, b in chosen_connections:
#     print(f"{d:.3f} -> {a} <-> {b}")


adj = defaultdict(list)
visited = set()

def count_components():
    visited.clear()
    count = 0
    for node in range(n):
        if node not in visited:
            count += 1
            stack = [node]
            visited.add(node)
            while stack:
                u = stack.pop()
                for v in adj[u]:
                    if v not in visited:
                        visited.add(v)
                        stack.append(v)
    return count

components = n
last_pair = None

for d, a, b in pairs:
    # Add the connection
    adj[a].append(b)
    adj[b].append(a)

    # Recompute number of connected components
    components = count_components()

    # Check if we reached a SINGLE component
    if components == 1:
        last_pair = (a, b)
        break

# Extract X-coordinates and multiply
x1 = coords[last_pair[0]][0]
x2 = coords[last_pair[1]][0]
print(x1, x2)

answer = x1 * x2
print(answer)
# found 25272 as expected for the test.txt