#ifndef __DLIST_H
#define __DLIST_H

typedef struct dl_list {
	struct dl_list *prev;
	struct dl_list *next;
} dl_list_t;

#ifndef offsetof
#define offsetof(type, member) ((long) &((type *) 0)->member)
#endif

#define dl_list_entry(item, type, member) \
    ((type *) ((char *) item - offsetof(type, member)))

#define dl_list_first(list, type, member) \
    (dl_list_empty((list)) ? NULL : \
     dl_list_entry((list)->next, type, member))

#define dl_list_last(list, type, member) \
    (dl_list_empty((list)) ? NULL : \
     dl_list_entry((list)->prev, type, member))

#define dl_list_for_each(item, list, type, member) \
    for (item = dl_list_entry((list)->next, type, member); \
         &item->member != (list); \
         item = dl_list_entry(item->member.next, type, member))

#define dl_list_for_each_safe(item, n, list, type, member) \
    for (item = dl_list_entry((list)->next, type, member), \
             n = dl_list_entry(item->member.next, type, member); \
         &item->member != (list); \
         item = n, n = dl_list_entry(n->member.next, type, member))

#define dl_list_for_each_reverse(item, list, type, member) \
    for (item = dl_list_entry((list)->prev, type, member); \
         &item->member != (list); \
         item = dl_list_entry(item->member.prev, type, member))

/* define a double-linked list */
#define DEFINE_DL_LIST(name) \
    struct dl_list name = { &(name), &(name) }


static inline void dl_list_init(struct dl_list *list)
{
	list->next = list;
	list->prev = list;
}

static inline void __dl_list_add(struct dl_list *new, struct dl_list *prev, struct dl_list *next)
{
	next->prev = new;
	new->next = next;
	
	new->prev = prev;
	prev->next = new;
}

/* Head insert. Stack implementation */
static inline void dl_list_add(struct dl_list *head, struct dl_list *new)
{
	__dl_list_add(new, head, head->next);
}

/* Tail insert. Queue implementation */
static inline void dl_list_add_tail(struct dl_list *head, struct dl_list *new)
{
	__dl_list_add(new, head->prev, head);
}

static inline void __dl_list_del(struct dl_list *prev, struct dl_list *next)
{
	prev->next = next;
	next->prev = prev;
}

static inline void dl_list_del(struct dl_list *n)
{
	__dl_list_del(n->prev, n->next);
	n->prev = NULL;
	n->next = NULL;
}

/* Replace old entry by new one */
static inline void dl_list_replace(struct dl_list *old, struct dl_list *new)
{
	new->next = old->next;
	new->next->prev = new;
	new->prev = old->prev;
	new->prev->next = new;
}

static inline int dl_list_empty(const struct dl_list *list)
{
    return list->next == list;
}

static inline unsigned int dl_list_len(const struct dl_list *list)
{
    struct dl_list *item;
    int count = 0;
    for (item = list->next; item != list; item = item->next)
        count++;
    return count;
}

#endif
