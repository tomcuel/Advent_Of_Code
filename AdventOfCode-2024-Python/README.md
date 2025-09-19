# Advent of Code 2024
> I've done the Advent of Code 2024 (AOC) in Python. It's basically an advent calendar composed of programming puzzles that you can solve in every type of language you want. A puzzle made give one star and each problem consist of two puzzles, a first one, and then a continuation of this one that is harder to solve. You have an input you need to download and parse to later get the expected result. The difficulty is generally increasing as each day passes. I didn't succeed to solve every puzzle (17p2, 21p2, 24p2). Here is the website of this challenge to see the problems : https://adventofcode.com/.

#### Tables of contents
* [Path Tree](#path-tree)
* [Virtual Environnment](#virtual-environnment)

## Path Tree
AdventOfCode-2024-Python/  
├── [day1](./day1/)  
├── [day2](./day2/)  
├── [day3](./day3/)  
├── [day4](./day4/)  
├── [day5](./day5/)  
├── [day6](./day6/)  
├── [day7](./day7/)  
├── [day8](./day8/)  
├── [day9](./day9/)  
├── [day10](./day10/)  
├── [day11](./day11/)  
├── [day12](./day12/)  
├── [day13](./day13/)  
├── [day14](./day14/)  
├── [day15](./day15/)  
├── [day16](./day16/)  
├── [day17](./day17/)  
├── [day18](./day18/)  
├── [day19](./day19/)  
├── [day20](./day20/)  
├── [day21](./day21/)  
├── [day22](./day22/)  
├── [day23](./day23/)  
├── [day24](./day24/)  
└── [day25](./day25/)  

## Virtual Environnment
Since the libraries are not installed on my Mac, I need to set up a virtual environment to access and use the required libraries, the process might be different depending on your device, here is how to do :

Creating the virtual environnment
```
python3 -m venv path/to/venv
```
Activating the virtual environnment
```
source path/to/venv/bin/activate
```
Downloading the librairies you need in the virtual environnment
```
python3 -m pip install what_you_need
```
Those are the one I needed during my different problem solving, including the based Python librairies
```
import re # for regular expressions
from collections import itertools # for combinations
from collections import defaultdict # for initialized dictionnaries
from collections import deque # for BFS algorithms
from heapq import heappush, heappop # for BFS algorithms
python3 -m pip install sympy # for function problem solving 
```
To temporarly deactivate the virtual environnment
```
deactivate 
```