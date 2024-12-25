# Opening the file in read mode
with open('input5.txt', 'r') as file:
    # splitting the pages inequalities and the upgrade lectures
    data = file.read().strip().split('\n\n')

# Getting the int values of the pages inequalities
page_ordering_rules = [[int(i) for i in page.split('|')] for page in data[0].split('\n')]
# Getting the int values of the upgrade lectures
upgrade_lectures = [[int(i) for i in upgrade.split(',')] for upgrade in data[1].split('\n')]


# filling a list of the middle number of every correct upgrade
correct_upgrades_number = [0] * len(upgrade_lectures)
for k, upgrade in enumerate(upgrade_lectures):
    # looking if all the number behind a certain number respect the pages inequalities conditions
    # and we shift the number we're checking to the right to check all the numbers behind it
    if all(
        any(upgrade[i] == page[0] and upgrade[j] == page[1] for page in page_ordering_rules)
        for i in range(len(upgrade)-1) for j in range(i + 1, len(upgrade))
    ) :
        correct_upgrades_number[k] = upgrade[len(upgrade)//2]


# printing the answer
print("Answer: ", sum(correct_upgrades_number))
# we found 143 for the test5.txt file as expected