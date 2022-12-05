#include "graph.h"
#include "dijkstra.h"
#include "astar.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main() {
	#define DIJKSTRA 0 // set to 0 to test A*
    #define TESTING 0  // 1 == enable debug message logging

	Graph G;
	scanf_s("%d", &G.V);
	// Allocate memory for the edges array
	G.edges = malloc(G.V * sizeof * G.edges);
	// create the edge lists
	for (int i = 0; i < G.V; i++) {
		int out_degree;
		scanf_s("%d", &out_degree);
		G.edges[i] = new_list();
		for (int j = 0; j < out_degree; j++) {
			Edge edge;
			scanf_s("%d,%d", &edge.to_vertex, &edge.weight);
			add_edge(&G.edges[i], edge);
		}
	}
	// init the in-degrees array to 0's.
	int* in_degrees = malloc(G.V * sizeof in_degrees);
	for (int i = 0; i < G.V; i++) {
		in_degrees[i] = 0;
	}

	// traverse the edge nodes and increment the in-degrees of the vertexes they point to
	for (int i = 0; i < G.V; i++) {
		EdgeNodePtr current = G.edges[i].head;
		while (current != NULL) {
			in_degrees[current->edge.to_vertex]++;
			current = current->next;
		}

	}

	if (TESTING) {
		//Print the in-degrees of each vertex.
		for (int i = 0; i < G.V; i++) {
			printf("Vertex %d has %d in-degrees\n", i, in_degrees[i]);
		}
	}



	int start_square;
	printf_s("Input a starting square\n");
	scanf_s("%d", &start_square);

	int dest_square;
	printf_s("input a destination square\n");
	scanf_s("%d" , &dest_square);

	int option;
	long total = 0; // total time taken by all tests
	int count = 0; // number of time tests for a given function
	long average;
	const int TEST_ITERATIONS = 50;
	clock_t start;

	while (count != TEST_ITERATIONS) {
		start = clock();

		// calculate the shortest paths
		if (DIJKSTRA) {
			ShortestPaths paths = dijkstra(G, start_square - 1);
			// print the shortest path from start to destination
			print_shortest_path_dijkstra(paths, start_square - 1, dest_square - 1);
			free_paths_dijkstra(&paths);
		}
		else { // A*
			ShortestPath astar_path = astar(G, start_square - 1, dest_square - 1);
			print_shortest_path_astar(astar_path, start_square - 1, dest_square - 1);
			free_path_astar(&astar_path);
		}

		// Calculate and print the time taken
		clock_t diff = clock() - start;
		long msec = diff * 1000 / CLOCKS_PER_SEC;
		total += msec;
		count += 1;
		printf("\nOperation took %d milliseconds\n", msec);
		if (count == TEST_ITERATIONS) {
			average = total / count;
			printf("Average time taken: %d\n", average);
		}
	}


	
	// free the in-degrees
	free(in_degrees);

	// free the edge lists
	for (int i = 0; i < G.V; i++) {
		EdgeNodePtr current = G.edges[i].head;
		while (current != NULL) {
			EdgeNodePtr to_free = current;
			current = current->next;
			free(to_free);
		}
	}
	
	free(G.edges);
	return 0;
}