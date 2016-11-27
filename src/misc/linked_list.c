#include "linked_list.h"

#include <assert.h>
#include <stdlib.h>

static struct ll_node *node_make(void *data)
{
  struct ll_node *node = malloc(sizeof(struct ll_node));
  if (!node)
    return NULL;

  node->data = data;
  node->next = NULL;

  return node;
}

static void node_destroy(struct ll_node *node, int data_destruction)
{
  if (!node)
    return;

  node_destroy(node->next, data_destruction);
  if (data_destruction)
    free(node->data);
  free(node);
}

static void first_insertion(struct linked_list *ll, void *data)
{
  assert(ll->first == NULL && ll->last == NULL);

  ll->first = node_make(data);
  ll->last = ll->first;
}

struct linked_list *list_make(void)
{
  return calloc(1, sizeof(struct linked_list));
}

void list_add_back(struct linked_list *ll, void *data)
{
  assert(ll);
  assert(data && "Insertion of NULL data is not allowed!");

  if (ll->first == NULL)
  {
    first_insertion(ll, data);

    return;
  }

  ll->last->next = node_make(data);
  ll->last = ll->last->next;
}

size_t list_size(struct linked_list *ll)
{
  assert(ll);

  if (!ll->first)
    return 0;

  assert(ll->first < ll->last);

  return (size_t)(ll->last - ll->first);
}

void *list_extract(struct linked_list *ll,
                   const void *data,
                   int (* compare_function)(const void *, const void *))
{
  assert(ll);
  assert(data && "Data can't be NULL.");

  struct ll_node *ptr = ll->first;
  while (ptr && compare_function(ptr->data, data))
    ptr = ptr->next;

  if (!ptr)
    return NULL;

  return ptr->data;
}

void list_remove(struct linked_list *ll,
                 struct ll_node *removed_node,
                 int destruction)
{
  assert(ll && removed_node);

  if (ll->first == removed_node)
  {
    ll->first = removed_node->next;
    return;
  }

  struct ll_node *ptr = ll->first;
  while (ptr && ptr->next != removed_node)
    ptr = ptr->next;

  assert(ptr && "The removed node need to be in the list.");

  ptr->next = removed_node->next;
  if (destruction)
  {
    free(removed_node->data);
    free(removed_node);
  }
}

void list_destroy(struct linked_list *ll, int data_destruction)
{
  assert(ll);

  node_destroy(ll->first, data_destruction);
  free(ll);
}