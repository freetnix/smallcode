#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dlist.h"

struct subscription {
	struct dl_list ev_q;
};

struct cust_event {
	struct dl_list list;
	struct subscription *s;	/* parent */
	char *data;
};


struct subscription g_subscriber;

void event_init(struct subscription *lq)
{
	dl_list_init(&lq->ev_q);
}

#define MAX_EVENTS_QUEUED	128
void event_delete(struct cust_event *ev)
{
	free(ev->data);
	ev->data = NULL;

	free(ev);
	ev = NULL;
}

struct cust_event *event_dequeue(struct subscription *lq)
{
	struct cust_event *ev = NULL;

	ev = dl_list_first(&lq->ev_q, struct cust_event, list);
	if (ev) {
		dl_list_del(&ev->list);
	}

	return ev;
}

int event_add(struct subscription *lq, char *data)
{
	struct cust_event *new = NULL;
	int len = dl_list_len(&lq->ev_q);

	if (len >= MAX_EVENTS_QUEUED) {
		struct cust_event *ev = NULL;

		/* Drop oldest entry */
		ev = event_dequeue(lq);
		if (!ev)
			return -1;
		event_delete(ev);
	}

	new = (struct cust_event *)malloc(sizeof(struct cust_event));
	if (new == NULL)
		return -1;

	dl_list_init(&new->list);
	new->data = strdup(data);

	dl_list_add_tail(&lq->ev_q, &new->list);
}

void event_dump(struct subscription *lq)
{
	struct cust_event *ev;

	dl_list_for_each(ev, &lq->ev_q, struct cust_event, list)
		printf("Queue: ev->data=[%s]\n", ev->data);

	return;
}

int main(int argc, char **argv)
{
	struct cust_event *ev = NULL;

	event_init(&g_subscriber);

	event_add(&g_subscriber, "msg1");
	event_add(&g_subscriber, "msg2");
	event_add(&g_subscriber, "msg3");
	event_add(&g_subscriber, "msg4");

	event_dump(&g_subscriber);


	ev = event_dequeue(&g_subscriber);
	if (ev) {
		printf("ev->data=[%s]\n", ev->data);
		event_delete(ev);
	}

	event_dump(&g_subscriber);
	return 0;
}

