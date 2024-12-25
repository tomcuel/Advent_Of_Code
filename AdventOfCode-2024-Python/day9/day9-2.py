# Opening the file in read mode
with open('input9.txt', 'r') as file:
    first_line = file.read().strip()

# saving all the first location of the files and the size of the files combo
# so that we don't have to detect them again each time, we try to make a move
first_location_file_ID = [0] * len(first_line)
size_file_ID = [0] * len(first_line)

def get_blocks(disk_map):
    global first_location_file_ID, size_file_ID

    blocks = []
    is_file = True
    number = 0
    for x in disk_map:
        x = int(x) # getting the number of file or spaces between files
        if is_file:

            first_location_file_ID[number] = len(blocks) # saving the first location of the file
            size_file_ID[number] = x # saving the size of the file

            blocks += [number] * x # adding the files
            number += 1
            is_file = False
        else:
            blocks += [None] * x # adding the spaces between files
            is_file = True
        
    return blocks

# only the make move things change in part 2
# we now need to move entire blocks of files of the same number the most to the left
def make_move(file_blocks_array) : 

    # getting the number of file groups to move 
    number_of_files = sum(1 for x in size_file_ID if x != 0)
    print("number_of_files", number_of_files)
    for number_to_move in range(number_of_files, -1, -1):
        # Find first free space that works
        free_space = 0 # --> size of the free space
        first_free = 0 # --> beginning of the free space
        # we want to move the file to the left, so it needs to verify those conditions
        while first_free < first_location_file_ID[number_to_move] and free_space < size_file_ID[number_to_move]:
            first_free = first_free + free_space
            free_space = 0 # --> initializing the free space
            # if there is a file, we look one step to the right
            while file_blocks_array[first_free] is not None:
                first_free += 1
            # if there is a free space, we look how big it is
            while first_free + free_space < len(file_blocks_array) and file_blocks_array[first_free + free_space] is None:
                free_space += 1

        # we don't want to move the file to the right
        if first_free >= first_location_file_ID[number_to_move]:
            continue

        # Move file by swapping block values
        for index in range(first_free, first_free + size_file_ID[number_to_move]):
            file_blocks_array[index] = number_to_move
        for index in range(first_location_file_ID[number_to_move], first_location_file_ID[number_to_move] + size_file_ID[number_to_move]):
            file_blocks_array[index] = None

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
# we find 2858 for the test9.txt file as expected 