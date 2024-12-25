# Opening the file
with open('input19.txt', 'r') as file:
    data = file.read().strip().split('\n\n')

strips = data[0].split(', ')
towels = data[1].split('\n')

# Memoization dictionary to store the result of the subproblems to fasten the computation
memo = {}

def number_of_design_formed(strips, towel):
    # Check if the result for the current towel substring is already computed
    if towel in memo:
        return memo[towel]
    
    # Base case: If the towel is empty, it means we've successfully formed it, so return 1
    if towel == "":
        return 1
    
    nb_valid_combo = 0
    # Now check if the towel can be formed by the strips, going from the starting point
    for strip in strips:
        # Check if a strip matches at the current position
        if towel.startswith(strip):
            # Add the result and look if there is a next step 
            nb_valid_combo += number_of_design_formed(strips, towel[len(strip):])
    
    # Store the result in memoization dictionary and return it
    memo[towel] = nb_valid_combo
    return nb_valid_combo

def count_possible_designs_combination(strips_patterns, towels_patterns):
    sum = 0
    for towel in towels_patterns:
        sum+= number_of_design_formed(strips_patterns, towel)
    return sum
   
# print the answer
answer = count_possible_designs_combination(strips,towels)
print("Answer:", answer)
# we found 16 for the test19.txt file as expected