#pragma once
#include "graph.h"

typedef struct shortestPath {
	int goal_distance;
	int* previous_vertices;
} ShortestPath;

ShortestPath astar(Graph graph, int start, int goal);
void print_shortest_path_astar(ShortestPath path, int start, int destination);
void free_path_astar(ShortestPath* path);