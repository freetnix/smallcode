#include <stdio.h>
#include <stdlib.h>

struct seq_stack {
	int capacity;
	int top;
	int *data;
};

struct seq_stack *stack_create(int size)
{
	if (size <= 0)
		return NULL;

	struct seq_stack *stack = malloc(sizeof(struct seq_stack));
	if (!stack)
		return stack;
	stack->capacity = size;
	stack->top = -1;
	stack->data = malloc(sizeof(int)*size);

	return stack;
}

int is_empty(struct seq_stack * stack)
{
	return stack->top == -1;
}

int is_full(struct seq_stack *stack)
{
	return stack->top == stack->capacity - 1;
}

int stack_size(struct seq_stack *stack)
{
	return stack->top + 1;
}

int stack_enlarge(struct seq_stack *stack)
{
	stack->data = realloc(stack->data, stack->capacity * 2 * sizeof(int));
	if (stack->data == NULL)
		return -1;

	stack->capacity *= 2;
	return 0;
}

void stack_push(struct seq_stack *stack, int item)
{
	if (is_full(stack) && stack_enlarge(stack) < 0)
		return;

	stack->data[++stack->top] = item;
}

int stack_pop(struct seq_stack *stack)
{
	if (is_empty(stack))
		return -1;

	return stack->data[stack->top--];
}

int stack_destory(struct seq_stack *stack)
{
	if (stack == NULL)
		return -1;

	if (stack->data) {
		free(stack->data);
		stack->data = NULL;
	}

	free(stack);
}

int main(int argc, char **argv)
{
	struct seq_stack *stack = stack_create(5);
	int data[]={1,2,3,4,5,6,7,8,9};
	int i=0;
	while (!is_full(stack))
		stack_push(stack, data[i++]);
	while (!is_empty(stack))
		printf("%d\n", stack_pop(stack) );

	printf("\n");
	while (i<9 && !is_full(stack))
		stack_push(stack, data[i++]);
	while (!is_empty(stack))
		printf("%d\n", stack_pop(stack) );
	return 0;
}

