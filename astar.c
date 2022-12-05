#include "astar.h"
#include "graph.h"
#include "minheap.h"
#include "dijkstra.h"
#include "stack.h"
#include <stdlib.h>
#include <math.h>


ShortestPath create_shortest_path() {
	ShortestPath tmp;
	tmp.goal_distance = 0;
	return tmp;
}

void print_shortest_path_astar(ShortestPath path, int start, int destination) {

		Stack path_stack = new_stack();

		// backtrack through previous vertices until we reach start, adding each vertex to the stack
		int current_vert = destination;

		while (current_vert != start) {
			int prev_vert = path.previous_vertices[current_vert]; // the vertex pointing to the current vertex (along the shortest path)
			stack_push(&path_stack, prev_vert); // add the previous vertex to the path stack
			current_vert = prev_vert; // go back one step along the shortest path to the previous vertex
		}

		// print the results
		printf_s("\nShortest path from square %d to square %d has a length of %d.\n", start, destination + 1, path.goal_distance);
		while (path_stack.top != NULL) {
			printf_s("Square %d --> ", stack_pop(&path_stack)+1);
		}
		printf_s("Square %d", destination+1);

}

int getRow(int vertex, int grid_length) {
	return vertex / grid_length; // integer division will round answer down to the row
}

int getCol(int vertex, int grid_length) {
	return vertex - (grid_length * (vertex / grid_length)); // integer division will round answer down 
}

// the heuristic takes the starting and ending vertex and returns the min possible distance between the two
int heuristic(Graph graph, int start, int end) {
	// straight line distance on a square grid
	int grid_length = (int)sqrt(graph.V); // get the side length of the 2D grid
	return abs(getRow(start, grid_length) - getRow(end, grid_length)) + abs(getCol(start, grid_length) - getCol(end, grid_length));
}

void free_path_astar(ShortestPath* path) {
	free(path->previous_vertices);
}

ShortestPath astar(Graph graph, int start, int goal) {
	const int INF = -1;
	// create distances array
	int* distances = malloc(graph.V * sizeof * distances);
	int* previous_vertices = malloc(graph.V * sizeof * previous_vertices);

	// For vertex n, best_guess[n] == distances[n] + heuristic(n). 
	// best_guess[n] is our current best guess as to how cheap a path 
	// could be from start to finish if it goes through n.
	int* best_guess = malloc(graph.V * sizeof * best_guess);

	//for each vertex v in Graph.Vertices:
	for (int i = 0; i < graph.V; i++) {
		distances[i] = INF;				// set distance[v] to infinity
		previous_vertices[i] = NULL;		// set prev[v] to null
		best_guess[i] = INF;
	}

	distances[start] = 0; // set the starting vertex distance to 0.
	best_guess[start] = heuristic(graph, start, goal);

	MinHeap open_set = create_minheap(graph.V); // initialise the min heap here
	add(&open_set, start, distances); // add starting vertex to the min heap 

	while (open_set.size != 0) {
		int closest_vertex = poll(&open_set, distances); // remove the smallest value from the open set


		if (closest_vertex == goal) { // we found it, so reconstruct the path
			ShortestPath path = create_shortest_path();
			path.goal_distance = distances[goal];
			path.previous_vertices = previous_vertices;

			free(distances);
			free_minheap(&open_set);
			free(best_guess);
			return path; // returns the paths to be printed by print_shortest_path_astar()
		}
		
		EdgeNodePtr current = graph.edges[closest_vertex].head;
		while (current != NULL) { // for each neighbour (edge) of the current vertex
			int edgeTo = current->edge.to_vertex;
			int weight = current->edge.weight;
			int tentative_score = distances[closest_vertex] + weight;
			// if the distance to the vertex is less than what we have stored in distances[], use the lower value
			if (distances[edgeTo] == -1 || distances[edgeTo] > tentative_score) {
				distances[edgeTo] = tentative_score;
				previous_vertices[edgeTo] = closest_vertex; 
				best_guess[edgeTo] = tentative_score + heuristic(graph, edgeTo, goal); // update our best guess of remaining distance from current point

				if (!minheap_contains(open_set, edgeTo)) {
					add(&open_set, edgeTo, distances); // add the vertex to the open set if it isn't already in there
				}
				
			}
			current = current->next;
		}

	}

	// open set is empty but we didn't reach the goal. failure. should never get here with a grid map.
	return create_shortest_path();





}