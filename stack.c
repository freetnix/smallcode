#include <stdio.h>                                  
#include <unistd.h>                                  
#include <stdlib.h>                                  
#include <string.h>                                  

#include "dlist.h"

struct link_stack {
	struct dl_list list;
	char *data;
	struct link_stack *top;
	int size;
};

struct link_stack g_link_stack;

int stack_init(void)
{
	memset(&g_link_stack, 0, sizeof(struct link_stack));
	dl_list_init(&g_link_stack.list);
}

int stack_push(struct link_stack *stk, const void *data)
{
	struct link_stack *lsn;

	lsn = (struct link_stack *)malloc(sizeof(struct link_stack));
	if (lsn == NULL)
		return -1;

	lsn->data = strdup(data);

	dl_list_add(&stk->list, &lsn->list);
	stk->top = dl_list_first(&stk->list, struct link_stack, list);
	stk->size++;

	/* success */
	return 0;
}

int stack_pop(struct link_stack *stk, char **data)
{
	if (stk->top == NULL || dl_list_empty(&stk->list) || stk->size <= 0) {
		printf("No more element!\n");
		return -1;
	}

	struct link_stack *elm = stk->top;

	if (elm->data) {
		strcpy(*data, elm->data); 
		free(elm->data);
	}
	dl_list_del(&elm->list);
	free(elm);

	stk->size--;
	stk->top = dl_list_first(&stk->list, struct link_stack, list);

	return 0;
}

static inline int stack_size(struct link_stack *stk)
{
	return stk->size;
}

static void stack_dump(struct link_stack *stk)
{
	int i = 0;
	struct link_stack *s = NULL;

	dl_list_for_each(s, &stk->list, struct link_stack, list) {
		printf("%d: %s\n", i++, s->data);
	}

	return;
}

int main(int argc, char **argv)
{
	char *data;

	data = malloc(20);

	if (data == NULL)
		return -1;

	bzero(data, 20);

	stack_init();

	stack_push(&g_link_stack, "123");
	stack_push(&g_link_stack, "456");
	stack_push(&g_link_stack, "789");
	stack_push(&g_link_stack, "abc");
	stack_push(&g_link_stack, "def");

	stack_dump(&g_link_stack);

	printf("top=[%s], data=%s, stack_size=[%d]\n", g_link_stack.top->data, data, g_link_stack.size);
	stack_pop(&g_link_stack, &data);
	printf("top=[%s], data=%s, stack_size=[%d]\n", g_link_stack.top->data, data, g_link_stack.size);
	stack_pop(&g_link_stack, &data);
	printf("top=[%s], data=%s, stack_size=[%d]\n", g_link_stack.top->data, data, g_link_stack.size);

	stack_dump(&g_link_stack);
#if 0
	printf("data=%s, stack_size=[%d]\n", data, g_link_stack.size);
	stack_pop(&g_link_stack, &data);
	printf("data=%s, stack_size=[%d]\n", data, g_link_stack.size);
	stack_pop(&g_link_stack, &data);
	printf("data=%s, stack_size=[%d]\n", data, g_link_stack.size);
	stack_pop(&g_link_stack, &data);
	printf("data=%s, stack_size=[%d]\n", data, g_link_stack.size);
	stack_pop(&g_link_stack, &data);
	printf("data=%s, stack_size=[%d]\n", data, g_link_stack.size);
#endif

	free(data);

	return 0;
}

