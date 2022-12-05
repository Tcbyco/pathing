#include "graph.h"
#include "dijkstra.h"
#include "minheap.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

ShortestPaths new_shortest_path() {
	ShortestPaths tmp;
	tmp.distances = NULL;
	tmp.prev_vertices = NULL;
	return tmp;
}

// calculates the shortest distances from start vertex to each other vertex, 
// and returns these distances along with a previous_vertices array
// for use when obtaining the final path.

ShortestPaths dijkstra(Graph graph, int start) {
	const int INF = -1; // infinity, used to represent distances that have not been calculated yet
	// create distances array
	int* distances = malloc(graph.V * sizeof * distances);
	int* previous_vertices = malloc(graph.V * sizeof * previous_vertices);
	MinHeap unvisited_set = create_minheap(graph.V);

	//for each vertex v in Graph.Vertices:
	for (int i = 0; i < graph.V; i++) {
		distances[i] = INF;				// set distance of v to infinity
		previous_vertices[i] = NULL;		// set prev to null			
		add(&unvisited_set, i, distances); // add each vertex to the unvisited set
	}
	distances[start] = 0; 
	update_value_in_minheap(&unvisited_set, start, distances);
	
	while (minheap_not_empty(unvisited_set)) { 
		
		// remove the vertex with the shortest distance
		int closest_vertex = poll(&unvisited_set, distances);

		// check each edge from the removed vertex, and update distances[] if necessary 
		EdgeNodePtr current = graph.edges[closest_vertex].head; 
		while (minheap_not_empty(unvisited_set) && current != NULL) {
			int edgeTo = current->edge.to_vertex;
			int weight = current->edge.weight;
			
			// if the edge points to a vertex that is still unvisited,
			if (minheap_contains(unvisited_set, edgeTo)) {
				// if the distance to the vertex is less than what we have stored in distances[], use the lower value
				if (distances[edgeTo] == INF || distances[edgeTo] > distances[closest_vertex] + weight) {
					distances[edgeTo] = distances[closest_vertex] + weight;
					previous_vertices[edgeTo] = closest_vertex;
					update_value_in_minheap(&unvisited_set, edgeTo, distances);
				}
			}
			current = current->next;
		}
	}
	// return the shortest paths
	ShortestPaths paths = new_shortest_path();

	paths.distances = distances;
	paths.prev_vertices = previous_vertices;

	free_minheap(&unvisited_set);
	return paths;
}

void free_paths_dijkstra(ShortestPaths* paths) {
	free(paths->distances);
	free(paths->prev_vertices);
}

void print_shortest_path_dijkstra(ShortestPaths paths, int start, int destination) {
	Stack path_stack = new_stack();

	// backtrack through previous vertices until we reach start, adding each vertex to the stack
	int current_vert = destination;
	
	while (current_vert != start) {
		int prev_vert = paths.prev_vertices[current_vert]; // the vertex pointing to the current vertex (along the shortest path)
		stack_push(&path_stack, prev_vert); // add the previous vertex to the path stack
		current_vert = prev_vert; // go back one step along the shortest path to the previous vertex
	}
	
	// print the results
	printf_s("Shortest path to Square %d has a length of %d.\n", destination+1, paths.distances[destination]);
	while (path_stack.top != NULL) {
		printf_s("Square %d --> ", stack_pop(&path_stack)+1);
	}
	printf_s("Square %d", destination+1);
	
}



