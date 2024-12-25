from collections import defaultdict
from itertools import combinations

# Opening the file
with open('input23.txt', 'r') as file:
    computer_connections = file.read().strip().split('\n')
computer_connections = [line.split('-') for line in computer_connections]

network = {}
for connection in computer_connections :
    if connection[0] not in network :
        network[connection[0]] = set()
    if connection[1] not in network :
        network[connection[1]] = set()
    network[connection[0]].add(connection[1])
    network[connection[1]].add(connection[0])
    
# looking for the sets of computers where each computer in the set is connected to the other two computers
computer_triangles = set()
for computer1 in network.keys() :
    for computer2 in network[computer1] :
        for computer3 in network[computer2].intersection(network[computer1]) :
            computer_triangles.add(tuple(sorted([computer1, computer2, computer3])))
'''
we found this set of interconnected computers as expected for the test23.txt file:
aq,cg,yn
aq,vc,wq
co,de,ka
co,de,ta
co,ka,ta
de,ka,ta
kh,qp,ub
qp,td,wh
tb,vc,wq
tc,td,wh
td,wh,yn
ub,vc,wq
'''

answer = 0 
for triangle in computer_triangles:
    if any(computer[0] == 't' for computer in triangle):
        answer += 1
# printing the answer
print("Answer : ", answer)
# we found 7 for the test23.txt file as expected