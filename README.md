# High Performance Energy-Balance Solver
A high performance solver to a puzzle game Energy Balance

## Requirements To Run
A windows computer, or compile source code in Linux.

## Description
1. Run the solver and input operation code 0 to generate a blank file.
2. Fill out the block with the numbers from the game screen, these commas are split tags.
3. For these sum constraint blocks, you must provide the direction by way of appending L, R, U, D.

## Example
1. game puzzle
![image](https://github.com/XiaoyangHou/energy-balance-solver/blob/main/test-puzzle.jpg)
2. fill out the blocks
![image](https://github.com/XiaoyangHou/energy-balance-solver/blob/main/puzzle%20test2.png)
3. run the solver and input operation code 1 to get a solution
![image](https://github.com/XiaoyangHou/energy-balance-solver/blob/main/solution.png)

## Algorithm
We use DFS (Depth First Search) to solve the problem.
By carefully constructing the order of search, we achieve a high-performance pruning algorithm.
