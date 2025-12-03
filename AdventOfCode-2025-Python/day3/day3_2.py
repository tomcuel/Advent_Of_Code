with open('input.txt', 'r') as f : 
    lines = f.read().strip().split('\n')


total = 0
for line in lines:
    digits = list(map(int, line))

    selected = []
    start_search = 0 

    for i in range(12):
        remaining = 11 - i # the remaining indexs to make sure we have a 12 digit number
        # find the max digits from start_search to end - remaining index to find to have a 12 digit number 
        idx = max(range(start_search, len(digits) - remaining), key=lambda i: digits[i])
        selected.append(digits[idx])
        start_search = idx+1

    number = int("".join(map(str, selected)))
    total += number
    # print(selected, number)


print(total)
# found 3121910778619 as expected for the test.txt