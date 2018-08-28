#ifndef __LIST_H__
#define __LIST_H__

#include "assert.h"
#define bool int

struct ListHead {
	struct ListHead *prev, *next;
};
typedef struct ListHead ListHead;
#define list_entry(ptr, type, member) \
	((type*)((char*)(ptr) -(long int) (&((type*)0)->member)))

static inline void
listadd(ListHead *prev, ListHead *next, ListHead *data) {
	assert(data != NULL);
	data->prev = prev;
	data->next = next;
	if (prev != NULL) prev->next = data;
	if (next != NULL) next->prev = data;
}

static inline void
listaddbefore(ListHead *list, ListHead *data) {
	assert(list != NULL);
	listadd(list->prev, list, data);
}

static inline void
listaddafter(ListHead *list, ListHead *data) {
	assert(list != NULL);
	listadd(list, list->next, data);
}

static inline void
listdel(ListHead *data) {
	assert(data != NULL);
	ListHead *prev = data->prev;
	ListHead *next = data->next;
	assert(prev != NULL);
	assert(prev->next == data);
	assert(next != NULL);
	assert(next->prev == data);
	if (prev != NULL) prev->next = next;
	if (next != NULL) next->prev = prev;
}
static inline void
listinit(ListHead *list) {
	assert(list != NULL);
	list->prev = list;
	list->next = list;
}

static inline bool
listempty(ListHead *list) {
	assert(list != NULL);
	return list == list->next;
}

#define list_foreach(ptr, head) \
	for ((ptr) = (head)->next; (ptr) != (head); (ptr) = (ptr)->next)

#endif

