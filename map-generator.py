# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""
import random

MAP_SIDE_LENGTH = 25
TOTAL_VERTICES = MAP_SIDE_LENGTH ** 2
MAX_EDGES = 4
MAX_WEIGHT = 10
edge_numbers = [i for i in range(1, MAX_EDGES)]
all_vertices = [i for i in range(1,TOTAL_VERTICES)]
all_weights = [i for i in range(1, MAX_WEIGHT)]

grid = [[i + (row * MAP_SIDE_LENGTH) for i in range(1, MAP_SIDE_LENGTH +1)] for row in range(MAP_SIDE_LENGTH)]
square_costs = [[random.choice(all_weights) for i in range(MAP_SIDE_LENGTH)] for i in range(MAP_SIDE_LENGTH)]


# the position of each square defines its edges
# north = [-1][]
# south = [+1][]
# east =[][+1]
# west = [][-1]

# 2D MAP GEN STARTS HERE

with open('map625.txt', 'w') as file:
    file.write(f"{TOTAL_VERTICES}\n")
    # go through each square in order and output its edges
    for row in range(MAP_SIDE_LENGTH):
        for col in range(MAP_SIDE_LENGTH):
            square = grid[row][col]
            edge_count = 0
            vertex_string = ""
            
            north_vertex = None
            north_weight = 0
            if row != 0:
                north_vertex = grid[row-1][col]
                north_weight = square_costs[row-1][col]
                edge_count+=1
                vertex_string += f"{north_vertex},{north_weight} "
            
            south_vertex = None
            south_weight = 0
            if row != MAP_SIDE_LENGTH -1:
                south_vertex = grid[row+1][col]
                south_weight = square_costs[row+1][col]
                edge_count+=1
                vertex_string += f"{south_vertex},{south_weight} "
                
            east_vertex = None
            east_weight = 0
            if col != MAP_SIDE_LENGTH -1:
                east_vertex = grid[row][col+1]
                east_weight = square_costs[row][col+1]
                edge_count+=1
                vertex_string += f"{east_vertex},{east_weight} "
            
            west_vertex = None
            west_weight = 0
            if col !=0:
                west_vertex = grid[row][col-1]
                west_weight = square_costs[row][col-1]
                edge_count+=1
                vertex_string += f"{west_vertex},{west_weight}"
            
            file.write(f"{edge_count}\n{vertex_string}\n")