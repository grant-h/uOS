#include <list.h>

void list_append(struct ll_node * entry, struct ll_node * head)
{
  // Previous of HEAD is the last node
  struct ll_node * last = head->prev;

  entry->next = head;
  entry->prev = last;
  last->next = entry;
  head->prev = entry;
}

void list_insert(struct ll_node * entry, struct ll_node * head)
{
  entry->next = head->next;
  entry->prev = head;
  head->next->prev = entry;
  head->next = entry;
}

void list_delete(struct ll_node * entry)
{
  struct ll_node * left = entry->prev, *right = entry->next;

  left->next = right;
  right->prev = left;

  entry->next = LIST_POISON;
  entry->prev = LIST_POISON;
}
