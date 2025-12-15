with open('input.txt', 'r') as f:
    lines = f.read().strip().split('\n')

lines={e.strip().split(':')[0]: e.strip().split(':')[1].strip().split(' ') for e in lines}

# path need to got through fft and dac, from svr to out
REQUIRED1 = "fft"
REQUIRED2 = "dac"
from functools import lru_cache
@lru_cache(None)
def find_path(node, seen_fft, seen_dac):
    """
    node       = current location
    seen_fft   = 1 if fft appeared earlier in path
    seen_dac   = 1 if dac appeared earlier in path
    """
    # If we reached out, is valid only if both mandatory nodes visited
    if node == "out":
        return int(seen_fft and seen_dac)
    
    # Update flags if we encounter fft or dac
    if node == REQUIRED1:
        seen_fft = 1
    if node == REQUIRED2:
        seen_dac = 1

    ans = 0
    for nxt in lines[node]: # return the number of path from this point to out 
        ans += find_path(nxt, seen_fft, seen_dac)
    return ans


print(find_path("svr", 0, 0))
# found 2 as expected for the test2.txt