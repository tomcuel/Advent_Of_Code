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
    
def are_interconnected(lan) :
    # all must be interconnected to each other
    for computer in lan :
        if (len(lan.difference(network[computer])) > 1) :
            return False
    return True

# looking for the sets of computers where each computer in the set is connected to the other two computers
biggest_party = []
for computer1 in network.keys() :
    if computer1[0] == 't':
        for computer2 in network[computer1] :
            # getting the intersection of the two sets and their original computers
            lan = network[computer1].intersection(network[computer2])
            lan.add(computer1)
            lan.add(computer2)

            # the biggest party is of size inferior to this reunion and each of its computers is interconnected
            if len(biggest_party) < len(lan) and are_interconnected(lan) :
                biggest_party = list(lan)

biggest_party.sort()
password = ','.join(biggest_party)
# printing the answer
print("Answer : ", password)
# we find "co,de,ka,ta" for the test23.txt file as expected