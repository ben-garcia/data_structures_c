#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct slist_node slist_node;
typedef struct slist slist;
typedef struct slist_iter slist_iter;

/* list prototypes */

slist *slist_create(size_t data_size);
slist *slist_create_from_file(const char *filename, size_t data_size,
                              void (*free)(void *));
void slist_set_free_function(slist *list, void (*freefn)(void *));
void slist_set_match_function(slist *list, bool (*matchfn)(void *, void *));
slist_node *slist_add(slist *list, void *data);
void *slist_find(slist *list, void *data);
bool slist_remove(slist *list, void *data);
void *slist_get_node_data(slist_node *node);
void slist_destroy(slist *list);

/* iterator prototypes */
slist_iter *slist_iter_create(slist *list);
slist_node *slist_iter_next(slist_iter *it);
void slist_iter_reset(slist *list, slist_iter *it);
void slist_iter_destroy(slist_iter *it);

#endif
