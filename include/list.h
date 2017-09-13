#ifndef LIST_H
#define LIST_H

#include <common.h>

/* A simple doubly-linked list implementation
 * Inspired by generic list usage in the Linux kernel
 *
 * Instead of having the list node containing the data,
 * we merely attach a linkage structure to a user defined
 * struct. Allows for a single list implementation while still
 * being expressive enough for any list node data.
 */

#define LIST_POISON ((struct ll_node *)0xdead0000)

struct ll_node {
  struct ll_node * next;
  struct ll_node * prev;
};

// This is a nice trick to initialize the list circularly to itself
// i.e. struct ll_node list = { &list, &list }
#define LIST_INIT(name) { &(name), &(name) }
#define LIST_DEFINE(name) struct ll_node name = LIST_INIT(name)

#define list_entry(ptr, type, member) \
  container_of(ptr, type, member)

#define list_for_each(pos, head) \
  for(pos = (head)->next; pos != head; pos = pos->next)

#define list_for_each_safe(pos, nxt, head) \
  for(pos = (head)->next, nxt = pos->next; pos != head; pos = nxt, nxt = pos->next)

void list_append(struct ll_node * entry, struct ll_node * head);
void list_insert(struct ll_node * entry, struct ll_node * head);
void list_delete(struct ll_node * entry);

#endif
