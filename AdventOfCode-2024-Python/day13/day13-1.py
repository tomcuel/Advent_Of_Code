# Opening the file in read mode
with open('input13.txt', 'r') as file:
    input = file.read().strip().split('\n\n')

number_max_of_clicks = 100

commands = []
for i,command in enumerate(input):
    command = command.split('\n')
    buttonA = command[0].split(' ')
    buttonB = command[1].split(' ')
    prize = command[2].split(' ')
    Xa = int(buttonA[2][2:-1])
    Ya = int(buttonA[3][2:])
    Xb = int(buttonB[2][2:-1])
    Yb = int(buttonB[3][2:])
    Xpri = int(prize[1][2:-1])
    Ypri = int(prize[2][2:])
    commands.append((Xa, Ya, Xb, Yb, Xpri, Ypri))

def get_token(command) :
    Xa, Ya, Xb, Yb, Xpri, Ypri = command
    least_amount_of_clicks_A = number_max_of_clicks+1 # --> set up the max since we need to find the least amount of clicks
    least_amount_of_clicks_B = number_max_of_clicks+1 # --> set up the max since we need to find the least amount of clicks
    # we need to find the least amount of clicks on each button so that : 
    # Xa + A_clicks + Xb * B_clicks = Xpri
    # Ya + A_clicks + Yb * B_clicks = Ypri
    print(Xa, Ya, Xb, Yb, Xpri, Ypri)
    for A_clicks in range(number_max_of_clicks):
        for B_clicks in range(number_max_of_clicks):
            if Xa * A_clicks + Xb * B_clicks == Xpri and Ya * A_clicks + Yb * B_clicks == Ypri:
                least_amount_of_clicks_A = min(least_amount_of_clicks_A, A_clicks)
                least_amount_of_clicks_B = min(least_amount_of_clicks_B, B_clicks)
    print(least_amount_of_clicks_A, least_amount_of_clicks_B)
    return (least_amount_of_clicks_A ,least_amount_of_clicks_B)



answer = 0
for command in commands:
    least_A, least_B = get_token(command)
    if not (least_A == number_max_of_clicks+1 or least_B == number_max_of_clicks+1):
        answer += 3 * least_A + least_B
# printing the answer
print("The answer is:", answer)
# We found 480 for the test13.txt file as expected