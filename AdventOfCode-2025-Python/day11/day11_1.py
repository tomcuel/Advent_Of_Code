with open('input.txt', 'r') as f:
    lines = f.read().strip().split('\n')

lines={e.strip().split(':')[0]: e.strip().split(':')[1].strip().split(' ') for e in lines}
# print(lines)

def find_path(curr):
    if curr == "out": # output
        return 1
    ans = 0
    for nxt in lines[curr]: # return the number of path from this point to out 
        ans += find_path(nxt)
    return ans

print(find_path("you"))
# found 5 as expected for the test1.txt