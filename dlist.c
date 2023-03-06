#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "dlist.h"

struct cus_info {
	struct dl_list list;

	void *data;
	//int size;
	int (*match)(const void *key1, const void *key2);
	void (*destroy)(void *data);
	//struct dl_list inf_list;
	//struct dl_list ev_queue;
};

struct cus_info g_cus_info;

int cus_init(void)
{
	memset(&g_cus_info, 0, sizeof(g_cus_info));
	dl_list_init(&g_cus_info.list);
}

struct cus_info *cus_add(char *data)
{
	struct cus_info *new = NULL;

	new = (struct cus_info *) malloc(sizeof(struct cus_info));

	if (new == NULL)
		return NULL;

	new->data = strdup(data);
	dl_list_add_tail(&g_cus_info.list, &new->list);

	return new;
}

void cus_del(struct cus_info *elm)
{
	if (elm == NULL)
		return;

	dl_list_del(&elm->list);
	free(elm->data);
	elm->data = NULL;
}

void cus_destroy(void)
{
	struct cus_info *s, *tmp;

	dl_list_for_each_safe(s, tmp, &g_cus_info.list, struct cus_info, list) {
		dl_list_del(&s->list);
		free(s->data);
		s->data = NULL;
	}
}

struct cus_info *cus_get(char *data)
{
	struct cus_info *s = NULL;

	dl_list_for_each(s, &g_cus_info.list, struct cus_info, list) {
		if (strcmp(s->data, data) == 0)
			return s;
	}

	return NULL;
}

void cus_dump(void)
{
	int i = 0;
	struct cus_info *s = NULL;

	dl_list_for_each(s, &g_cus_info.list, struct cus_info, list) {
		printf("%d: %s\n", i++, s->data);
	}

	return;
}

int main(int argc, char **argv)
{
	struct cus_info *a = NULL;

	cus_init();
	cus_add("123");
	cus_add("456");
	cus_add("789");
	cus_add("abc");
	cus_dump();

	a = cus_get("456");
	if (a != NULL) {
		printf("Find 456, delete it!\n");
		cus_del(a);
	}
	
	cus_dump();
	cus_destroy();
	cus_dump();

	return 0;
}
