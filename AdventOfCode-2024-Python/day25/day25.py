# Opening the file
with open('input25.txt', 'r') as file:
    file = file.read().strip().split('\n\n')


# parsing the input to get the values of the locks and keys in height
def parse(item):
    is_lock = False
    if item[0][0] == '#' : 
        is_lock = True

    values = []
    # converting the lock to a list of values of height
    if is_lock : 
        for j in range(5):
            for i in range(7):
                if item[i][j] == ".":
                    values.append(i-1)
                    break
    
    # converting the key to a list of values of height
    else : 
        for j in range(5):
            for i in range(6, -1, -1):
                if item[i][j] == ".":
                    values.append(5 - i)
                    break
    
    return values, is_lock


# getting the locks and keys values 
locks = []
keys = []
for item in file : 
    values, is_lock = parse(item.split('\n'))
    if is_lock : 
        locks.append(values)
    else :
        keys.append(values)


# finding the number of keys that can open the locks without overlapping
answer = 0
for lock in locks : 
    for key in keys : 
        is_good_key = True
        for i in range(5):
            if lock[i] + key[i] > 5 : # doesn't overlap
                is_good_key = False
                break

        if is_good_key :
            answer += is_good_key


# printing the answer
print("Answer:", answer) 
# we found 3 as expected for the test25.txt file