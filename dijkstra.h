#pragma once
typedef struct shortestPaths {
	int* distances;      // Shortest distances to each vertex
	int* prev_vertices;   // The vertext pointing to each vertex - for use when tracing back the shortest path
} ShortestPaths;

ShortestPaths new_shortest_path();
ShortestPaths dijkstra(Graph graph, int start);
void print_shortest_path_dijkstra(ShortestPaths paths, int start, int destination);
void free_paths_dijkstra(ShortestPaths* paths);