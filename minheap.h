#pragma once
#include <stdbool.h>

typedef struct minHeap {
	int capacity; // max size of the heap
	int size;     // current number of items in the heap
	int* items;  // array of items in the heap
	int* locations; // array used to track indexes of elements in the heap. -1 == not present.
} MinHeap;

MinHeap create_minheap(int capacity);
void add(MinHeap* self, int item, int* distances);
int poll(MinHeap* self, int* distances);
bool minheap_contains(MinHeap self, int item);
bool minheap_not_empty(MinHeap self);
void update_value_in_minheap(MinHeap* self, int edgeTo, int* distances);
void free_minheap(MinHeap* self);
