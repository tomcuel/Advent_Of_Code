# Opening the file in read mode
with open('input9.txt', 'r') as file:
    first_line = file.read().strip()

def get_blocks(disk_map):
    blocks = []
    is_file = True
    number = 0
    for x in disk_map:
        x = int(x) # getting the number of file or spaces between files
        if is_file:
            blocks += [number] * x # adding the files
            number += 1
            is_file = False
        else:
            blocks += [None] * x # adding the spaces between files
            is_file = True
        
    return blocks

def make_move(file_blocks_array) : 
    # finding the first free space
    first_free_space = 0
    while file_blocks_array[first_free_space] != None:
        first_free_space += 1

    # making the first move 
    i = len(file_blocks_array) - 1
    while file_blocks_array[i] == None:
        i -= 1
    # we go to the first occupied space (it's the last number of the file line)
    '''02X111....222X.'''
    # for this line it correspond to the X
    
    # while we can move the file to the first free space
    while i >= first_free_space: 
        file_blocks_array[first_free_space] = file_blocks_array[i] # making the swap
        file_blocks_array[i] = None # making the last place free
        # moving to the next free space from the left
        while file_blocks_array[first_free_space] != None:
            first_free_space += 1
        # moving to the next occupied space from the right 
        while file_blocks_array[i] == None:
            i -= 1

    return file_blocks_array

def get_sum(file_blocks_array) : 
    answer = 0
    for i, x in enumerate(file_blocks_array) : 
        # apart from the empty space, the value of a file is the number of the file multiplied by the position of the file
        if x != None:
            answer += i * x
    return answer

# printing the answer 
print("Answer : ", get_sum(make_move(get_blocks(first_line))))
# we find 1928 for the test9.txt file as expected 