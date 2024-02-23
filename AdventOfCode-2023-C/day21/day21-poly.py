import argparse
import numpy as np


Point = tuple[int, int]

DIRECTIONS: list[Point] = [(1, 0), (-1, 0), (0, 1), (0, -1)]
TOTAL_STEPS = 26_501_365
N_TILES = 9


def main():
    # Open the file called "input.txt" in the same directory as this file.
    input_file = open("input21.txt", 'r')
    # Read the file into a list where every element is a line from the file in order.
    # The lines are stored as strings.
    lines = input_file.read().strip().split('\n')
    # Close the file before we start solving.

    input_file.close()

    print(f"part 1 solution: {part_one(lines)}")
    print(f"part 2 solution: {part_two(lines)}")


def bfs(lines: list[str], start: Point) -> dict[Point, int]:
    rows = len(lines)
    cols = len(lines[0])

    steps: dict[Point, int] = {}

    step = 0
    to_visit: list[Point] = [start]
    while to_visit:
        new_to_visit = []

        for x, y in to_visit:
            if x < 0 or x >= cols:
                continue
            if y < 0 or y >= rows:
                continue
            if lines[x][y] == "#":
                continue
            if (x, y) in steps:
                continue

            steps[(x, y)] = step

            for dx, dy in DIRECTIONS:
                new_to_visit.append((x + dx, y + dy))

        to_visit = new_to_visit
        step += 1

    return steps


def find_start(lines: list[str]) -> Point:
    start = (-1, -1)
    for y, line in enumerate(lines):
        for x, char in enumerate(line):
            if char == "S":
                start = (x, y)
    assert start[0] >= 0 and start[1] >= 0
    return start

def part_one(lines: list[str]) -> int:
    # find starting position
    start = find_start(lines)

    # count squares reachable after 64 steps
    steps = bfs(lines, start)
    return sum([1 for step in steps.values() if step % 2 == 0 and step <= 64])


def part_two(lines: list[str]) -> int:
    # tile lines to N_TILES x N_TILES
    lines_tiled = []
    for line in N_TILES * lines:
        lines_tiled.append(N_TILES * line)

    # compute three sizes
    input_size = len(lines)
    center_offset = N_TILES // 2 * input_size
    sx, sy = find_start(lines)
    start_tiled = (sx + center_offset, sy + center_offset)
    steps = bfs(lines_tiled, start_tiled)

    offset = TOTAL_STEPS % input_size
    xs = list(range(3))
    ys = []
    for x in xs:
        x_scaled = x * input_size + offset
        ys.append(
            sum(
                [
                    1
                    for step in steps.values()
                    if step % 2 == x_scaled % 2 and step <= x_scaled
                ]
            )
        )

    # visualize results
    print_steps = 3 * 131 + 65
    for y in range(N_TILES):
        for x in range(N_TILES):
            tile_count = 0
            for i in range(input_size):
                for j in range(input_size):
                    point = (input_size * x + i, input_size * y + j)
                    if (
                        point in steps
                        and steps[point] % 2 == print_steps % 2
                        and steps[point] <= print_steps
                    ):
                        tile_count += 1
            print(f"{tile_count:4d} ", end="")
        print()

    a, b, c = np.polyfit(xs, ys, 2)
    total_steps_scaled = TOTAL_STEPS // input_size
    return round(a * total_steps_scaled**2 + b * total_steps_scaled + c)


if __name__ == "__main__":
    main()
