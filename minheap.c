#include "minheap.h"
#include <stdlib.h>
#include <stdbool.h>

MinHeap create_minheap(int capacity) {
	MinHeap tmp;
	tmp.capacity = capacity; // used to grow the heap if needed, not implemented in this version as it's not needed for my graph scenario. 
	tmp.size = 0;
	tmp.items = malloc(capacity * sizeof * tmp.items);
	tmp.locations = malloc(capacity * sizeof * tmp.locations);
	for (int i = 0; i < capacity; i++) { // initialise all values to 0
		tmp.items[i] = 0;    // initialise the heap array to zero for easier debugging
		tmp.locations[i] = -1; // -1 == not present in heap
	}
	return tmp;
}

bool minheap_not_empty(MinHeap self) {
	return self.size > 0;
}

int get_left_child_index(MinHeap self, int parent_index) {
	return 2 * parent_index + 1;
}

int get_right_child_index(MinHeap self, int parent_index) {
	return 2 * parent_index + 2;
}

int get_parent_index(MinHeap self, int child_index) {
	return (child_index - 1) / 2;
}

bool has_left_child(MinHeap self, int index) {
	return get_left_child_index(self, index) < self.size;
}

bool has_right_child(MinHeap self, int index) {
	return get_right_child_index(self, index) < self.size;
}

bool has_parent(MinHeap self, int index) {
	if (index <= 0) {
		return false;
	}
	else {
		return get_parent_index(self, index) >= 0;
	}
	
}

int get_left_child_item(MinHeap self, int index) {
	return self.items[get_left_child_index(self, index)];
}

int get_right_child_item(MinHeap self, int index) {
	return self.items[get_right_child_index(self, index)];
}

int get_parent_item(MinHeap self, int index) {
	return self.items[get_parent_index(self, index)];
}

void swap(MinHeap* self, int first_index, int second_index) {
	int first_item = self->items[first_index];  // used to swap positions in the locations array
	int second_item = self->items[second_index];

	// swap the items in the heap
	int temp = self->items[first_index];
	self->items[first_index] = self->items[second_index];
	self->items[second_index] = temp;
	
	// update the locations array so we can keep track of the new item positions in the heap
	temp = self->locations[first_item];
	self->locations[first_item] = self->locations[second_item];
	self->locations[second_item] = temp;
}

void heapify_up(MinHeap* self, int* distances) {
	int index = self->size - 1; 
	while (has_parent(*self, index) && distances[get_parent_item(*self, index)] != distances[self->items[index]] && ((distances[get_parent_item(*self, index)] == -1) || distances[get_parent_item(*self, index)] > distances[self->items[index]])){
		swap(self, get_parent_index(*self, index), index); // if the parent distance is bigger than current item, swap them. Note that this swaps the values, not the indices.
		index = get_parent_index(*self, index); // move up to the parent (which now has the value that we bubbled up)
	}
}

void heapify_down(MinHeap* self, int* distances) {
	int index = 0;
	while (has_left_child(*self, index)) {
		int smaller_child_index = get_left_child_index(*self, index);
		if ((has_right_child(*self, index)) && distances[get_left_child_item(*self, index)] == -1 || distances[get_right_child_item(*self, index)] < distances[get_left_child_item(*self, index)] && distances[get_right_child_item(*self, index)] != -1) {
			smaller_child_index = get_right_child_index(*self, index); // if the right child exists and is smaller than the left child, use it instead
		}

		if (distances[self->items[index]] == distances[self->items[smaller_child_index]]) {
			break; // both have the same distance (including infinite distance case), so no need to swap
		}

		if (distances[self->items[index]] != -1 && (distances[self->items[smaller_child_index]] == -1 || distances[self->items[index]] < distances[self->items[smaller_child_index]])) {
			break; // if current value is smaller than its children, we have finished rebalancing the heap.
		}
		else if (distances[self->items[index]] == -1) {
			// if current is infinity and the child isn't, swap them
			swap(self, index, smaller_child_index);
		}
		else {
			swap(self, index, smaller_child_index); // the child is smaller, so swap the parent and child.
		}
		index = smaller_child_index; // move down to the child's position (which now contains the value we are moving down) and repeat the process
	}
}

//above, and for the other functions here, make sure that a) we consider distance of -1 to be always bigger. 
//this should apply for bubbling up (if parent bigger, swap), bubbling down (-1 is not smaller and should not result in swapping with an infinte distance child)
//was this an issue with astar already, which used -1? only noticed issue because dijkstra was getting capped at 999 distance which presumable fucked up the ordering of stuff
//in the stack.
//
//when this is sorted, redo and then continue perf testing.

int poll(MinHeap* self, int* distances) {
	if (self->size == 0) {
		return -1; // the heap is empty
	}
	int item = self->items[0];
	self->items[0] = self->items[self->size - 1];
	self->size--;
	self->locations[item] = -1;
	heapify_down(self, distances);
	return item;
}

void add(MinHeap* self, int item, int* distances) {
	// add the item if it isn't already in there.
	if (self->locations[item] == -1) {
		self->items[self->size] = item;
		self->locations[item] = self->size;
		self->size++;
		heapify_up(self, distances);
	}
}

bool minheap_contains(MinHeap self, int item) {
	return self.locations[item] != -1;
}

void update_value_in_minheap(MinHeap* self, int item, int* distances) {
	int item_index = self->locations[item];
	// if a parent exists and has greater distance than the item we're updating, swap them.
	while (has_parent(*self, item_index) && ( (distances[get_parent_item(*self, item_index)] == -1) || distances[get_parent_item(*self, item_index)] > distances[item]) ) {
		int parent_index = get_parent_index(*self, item_index);
		swap(self, item_index, parent_index);
		item_index = parent_index;
	}

	while (has_left_child(*self, item_index)) {
		int smaller_child_index = get_left_child_index(*self, item_index);
		if ((has_right_child(*self, item_index)) && distances[get_right_child_item(*self, item_index)] < distances[get_left_child_item(*self, item_index)] && distances[get_right_child_item(*self, item_index)] != -1) {
			smaller_child_index = get_right_child_index(*self, item_index); // if the right child exists and is smaller than the left child, use it instead
		}

		if (distances[self->items[item_index]] == distances[self->items[smaller_child_index]]) {
			break; // both have the same distance (including infinite distance case), so no need to swap
		}

		if (distances[self->items[smaller_child_index]] == -1 || distances[self->items[item_index]] < distances[self->items[smaller_child_index]]) {
			break; // if current value is smaller than its children, we have finished rebalancing the heap.
		}
		else if (distances[self->items[item_index]] == -1) {
			// if current is infinity and the child isn't, swap them
			swap(self, item_index, smaller_child_index);
		}
		else {
			swap(self, item_index, smaller_child_index); // the child is smaller, so swap the parent and child.
		}
		item_index = smaller_child_index; // move down to the child's position (which now contains the value we are moving down) and repeat the process
	}
}

void free_minheap(MinHeap* self) {
	free(self->items);
	free(self->locations);
}