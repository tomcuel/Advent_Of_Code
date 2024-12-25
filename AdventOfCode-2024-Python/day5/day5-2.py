# Opening the file in read mode
with open('input5.txt', 'r') as file:
    # splitting the pages inequalities and the upgrade lectures
    data = file.read().strip().split('\n\n')

# Getting the int values of the pages inequalities
page_ordering_rules = [[int(i) for i in page.split('|')] for page in data[0].split('\n')]
# Getting the int values of the upgrade lectures
upgrade_lectures = [[int(i) for i in upgrade.split(',')] for upgrade in data[1].split('\n')]


# boolean list to know if the upgrade is correct or not
def is_upgrade_wrong(upgrade, page_ordering_rules):
    # looking if all the number behind a certain number respect the pages inequalities conditions
    # and we shift the number we're checking to the right to check all the numbers behind it
    if not all(
        any(upgrade[i] == page[0] and upgrade[j] == page[1] for page in page_ordering_rules)
        for i in range(len(upgrade)-1) for j in range(i + 1, len(upgrade))
    ) :
        return True
    return False


# function to rearrange the wrong upgrades line 
def rearrange_line(upgrade, page_ordering_rules):
    corrected_upgrade = []
    # we need to find the first number behind the index, that has all the number of the upgrade behind it respecting the pages inequalities conditions
    # then we place it at the index position and we continue the process until we reach the end of the upgrade by also changing the upgrade order
    while upgrade:
        for i,num in enumerate(upgrade):
            if all(
                any(num == page[0] and other == page[1] for page in page_ordering_rules)
                for other in upgrade[i + 1:]
            ):
                # Remove the number from the original list and add it to the corrected list
                corrected_upgrade.append(num)
                upgrade.pop(i) 
                break
    return corrected_upgrade


# getting the final result by summing the middle number of the correct upgrades and the rearranged wrong upgrades
answer = 0
for k, upgrade in enumerate(upgrade_lectures):
    # we need to count only the wrong upgrade lines and to rearrange the line to make it correct and then add the middle number to the answer
    if is_upgrade_wrong(upgrade, page_ordering_rules):
        corrected_upgrade = rearrange_line(upgrade, page_ordering_rules)
        answer += corrected_upgrade[len(corrected_upgrade)//2]


# printing the answer
print("Answer: ", answer)  
# we found 123 for the test5.txt file as expected