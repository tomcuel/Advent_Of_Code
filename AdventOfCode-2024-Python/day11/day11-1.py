# Opening the file in read mode
with open('input11.txt', 'r') as file:
    line = file.read().strip().split(' ')

# number of blinking for this part 
nb_blinking = 25

# loop for the number of blinking
for i in range (nb_blinking): 
    temp_line = []
    # applying the rules for each number in the line
    for number in line : 

        # a number 0 will be replaced by 1
        if number == '0':
            temp_line.append('1')

        # if the number is of even length, we split it in half
        elif len(number) % 2 == 0:
            left_stone = number[0:len(number)//2]
            temp_line.append(left_stone)
            right_stone = number[len(number)//2:]
            corrected_right_stone = ''
            index = 0
            while True : 
                if right_stone[index] == '0' and index < len(right_stone) - 1:
                    index += 1
                else : 
                    break
            corrected_right_stone = right_stone[index:]
            temp_line.append(corrected_right_stone)

        # otherwise, we multiply the number by 2024
        else : 
            number_x = int(number) * 2024 
            temp_line.append(str(number_x))

    line = temp_line

# print the answer
print("Answer:", len(line))
# we found 55312 for the test11.txt file as expected