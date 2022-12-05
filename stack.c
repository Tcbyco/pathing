#include <stdlib.h>
#include "stack.h"

//  stack used for printing shortest paths 

Stack new_stack() {
	Stack tmp;
	tmp.top = NULL;
	return tmp;
}

void stack_push(Stack* self, int vertex) {
	StackNodePtr new_node = malloc(sizeof * new_node);  // init a new node using the vertex
	new_node->vertex = vertex;
	new_node->next = self->top;
	self->top = new_node;  // put the new node on the top of the stack
}


int stack_pop(Stack* self) {
	int top_val = NULL;
	if (self->top != NULL) {
		top_val = self->top->vertex;
		StackNodePtr to_free = self->top;
		if (self->top->next != NULL) {
			self->top = self->top->next;
		}
		else {
			self->top = NULL;
		}

		free(to_free);
		return top_val;
	}
}