# 8-puzzle-solver
Solves the classic 8 puzzle problem using the A* search algorithm 

Takes initial state and goal state as input. 

Example:

0 1 2    
3 4 5     
6 7 8
    
    (0 represents a blank tile)
    
Outputs the optimal number of moves required to reach the goal state from the initial state. Uses two heuristic functions (Number of mismatches, Total Manhattan distance) sequentially for A* and compares their performace (number of nodes expanded and maximum size of the frontier at any point).
