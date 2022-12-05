#pragma once
typedef struct stackNode {
	int vertex;
	struct stackNode* next;
} *StackNodePtr;

typedef struct stack {
	StackNodePtr top;
} Stack;

Stack new_stack();
void stack_push(Stack* self, int vertex);
int stack_pop(Stack* self);