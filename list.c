struct my_list {
	void *data;
	list_t *head;
};

typedef struct list {
	struct list *prev;
	struct list *next;
} list_t;

/* initialize */
#define LIST_INIT(name) {&name, &name};
#define LIST_HEAD(name) struct list name = LIST_INIT(name)

/* head insert as 'stack' */
void list_add(list_t new, list_t head)
{
	new->next = head;
	new->prev = head->prev;
	head->prev->next = new;
	head->prev = new;
}

void __list_add(list_t new, list_t prev, list_t next)
{
	next->prev = new;
	new->next = next;
	
	new->prev = prev;
	prev->next = new;
	
}

void list_add(list_t new, list_t head)
{
	__list_add(new, head, head->next);
}

/* tail insert as 'queue' */
void list_add_tail(list_t new, list_t head)
{
	__list_add(new, head->prev, head);
}

void __list_del(list_t prev, list_t next)
{
	prev->next = next;
	next->prev = prev;
}

void list_del(list_t n)
{
	__list_del(n->prev, n->next);
	n->prev = NULL;
	n->next = NULL;
}
