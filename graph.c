#include "graph.h"
#include <stdlib.h>

EdgeList new_list() {
	EdgeList tmp;
	tmp.head = NULL;
	return tmp;
}

EdgeNodePtr add_edge(EdgeList *self, Edge e) {
	EdgeNodePtr new_node = malloc(sizeof * new_node);
	new_node->edge = e;
	new_node->next = self->head;
	self->head = new_node;

}



