# Opening the file
with open('input19.txt', 'r') as file:
    data = file.read().strip().split('\n\n')

strips = data[0].split(', ')
towels = data[1].split('\n')

def can_form_design(strips, towel):
    # function that tell if it's possible to have a combination starting from towel[start]
    def backtrack(start):
        # if we have reached the end of the towel, we have found a valid design
        if start == len(towel): 
            return True
        # now we will check if the towel can be formed by the strips, going from the starting point
        for strip in strips:
            if towel[start:].startswith(strip):  # Check if a strip matches at the current position
                if backtrack(start + len(strip)): # look if there is a next step 
                    return True
        # no match means that there is a problem 
        return False 

    # return from going from the beginning of the towel 
    return backtrack(0)

def count_possible_designs(strips_patterns, towels_patterns):
    sum = 0
    for towel in towels_patterns:
        if can_form_design(strips_patterns, towel):
              sum += 1
    return sum
   
# print the answer
answer = count_possible_designs(strips,towels)
print("Answer:", answer)
# we found 6 for the test19.txt file as expected
