#ifndef LPTABLE_H
#define LPTABLE_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct lptable_entry lptable_entry;
typedef struct lptable lptable;
typedef struct lptable_iter lptable_iter;

/* prototypes */

lptable *lptable_create(size_t data_size);
lptable *lptable_create_from_file(const char *filename, size_t data_size,
                                void (*frefn)(void *));
bool lptable_insert(lptable *table, const char *key, void *value);
bool lptable_insert_and_replace(lptable *table, const char *key, void *value);
void *lptable_find(lptable *table, const char *key);
bool lptable_remove(lptable *table, const char *key);
const char *lptable_get_entry_key(lptable_entry *entry);
void *lptable_get_entry_value(lptable_entry *entry);
void lptable_set_freefn(lptable *table, void (*freefn)(void *));
void lptable_destroy(lptable *table);

/* iterator prototypes */

lptable_iter *lptable_iter_create(lptable *table);
lptable_entry *lptable_iter_next(lptable_iter *it);
void lptable_iter_reset(lptable_iter *it);
void lptable_iter_destroy(lptable_iter *it);

#endif
