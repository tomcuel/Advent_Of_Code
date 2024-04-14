with open("input16.txt") as fin:
    raw_data = fin.read().strip()


# Conpacket_version t the hexadecimal data to binary
data = bin(int(raw_data, base=16))[2:]
data = data.zfill(-(-len(data)//4) * 4)


# function that returns the number corresponding to the type_ID and the value array given 
# for example, if the type_ID is 0, it returns the sum of the values in the values array
def operate(typeID, values):
    #return the sum of the values
    if typeID == 0:
        return sum(values)

    #return the product of the values
    if typeID == 1:
        p = 1
        for v in values:
            p *= v
        return p

    #return the min of the values
    if typeID == 2:
        return min(values)
    
    #return the max of the values
    if typeID == 3:
        return max(values)
    
    #no need to treat type_ID=4 since it does different things and we don't need this function for it 

    #compare the two differents values of the values array, but there is the need for it to be only of lenght 2
    if typeID == 5:
        assert len(values) == 2
        return int(values[0] > values[1])

    #compare the two differents values of the values array, but there is the need for it to be only of lenght 2
    if typeID == 6:
        assert len(values) == 2
        return int(values[0] < values[1])

    if typeID == 7:
        assert len(values) == 2
        return int(values[0] == values[1])


#function that parse the binary array input and return the answer of the puzzle 
# i represents the starting index of the parsing 
# j represents the end of the parsing
#cwe will not parse the data each time and gets some parts, we will update some parameters (i and j) to know where we're in the data
def parse(i, j=-1):

    if i == j:
        return None, None

    # Not useful bits anymore
    if i > len(data) - 4:
        return None, None

    # Get the packet_version soon and type_IDI
    packet_version  = int(data[i:i+3], base=2)
    typeID = int(data[i+3:i+6], base=2)

    # Literal packet : getting the value associated with the packet
    if typeID == 4:
        i += 6
        num_str = ""
        end = False
        # when we have a zero at the beginning of the group, we're at the end of the packet, and then we break the loop
        while not end:
            if data[i] == "0":
                # Last packet
                end = True

            num_str += data[i+1:i+5]
            i += 5

        val = int(num_str, base=2)
        return val, i

    # Operator packet
    # with values inside
    sub_packs = []
    next_start = None  # A value to return

    # getting what type of action we need to do with the packet depending on this value
    lenID = data[i+6]

    # if the packet is of type 0 : the 15 next bits represent the number of bits that are inside the next packet
    # if we also make sure that we can extract the number of bits correctly, that we don't go out of the array
    if lenID == "0":
        # 15 bits representing how many bits are inside
        num_bits = int(data[i+7:i+22], base=2)
        end = i + 22 + num_bits
        index = i + 22
        prev_index = None
        while index != None:
            prev_index = index
            x, index = parse(index, j=end)
            sub_packs.append(x)
        sub_packs = sub_packs[:-1]  # Remove last None
        next_start = prev_index

    # if the packet is of len_type 1 : the 11 next bits represent the number of bits that are inside the next packet
    # if we also make sure that we can extract the number of bits correctly, that we don't go out of the array
    else: # lenID == "1"
        # 11 bits representing how many packets are inside
        rem_sub_packs = int(data[i+7:i+18], base=2)
        index = i + 18
        while rem_sub_packs > 0:
            x, index = parse(index)
            rem_sub_packs -= 1
            sub_packs.append(x)

        next_start = index

    # Process the operations
    return operate(typeID, sub_packs), next_start

# printing the answer of the puzzle
ans = parse(0)[0]
print(ans)
