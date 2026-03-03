#include "hash_table.h"
#include "arena.h"
#include <stdalign.h>
#include <stdio.h>

int main(void) {
  printf("=========hash_table example========\n");

  arena *arena;
  hash_table *chars;
  hash_table_iterator *iterator;
  hash_table_entry *entry;
  char *value;
  char *key;

  arena_create(&arena, KB(4));
  hash_table_create(&chars, 32, NULL, arena);

  printf("inserting a-z\n");
  for (char c = 'a'; c <= 'z'; c++) {
    char *c_ptr = arena_alloc(arena, sizeof(char) + 1, alignof(char *), 0);
    snprintf(c_ptr, 2, "%c", c);
    hash_table_insert(chars, c_ptr, c_ptr);
  }

  printf("hash table size: %d\n\n", hash_table_size(chars));
  printf("sarching for 'c', found 0(yes), 1(no): %d\n\n", hash_table_lookup(chars, "c", (void **)&value));

  hash_table_iterator_create(&iterator, chars);

  printf("hash table contents: ");
  while ((hash_table_iterator_next(iterator, &entry)) == 0) {
    hash_table_entry_key(entry, &key);
    hash_table_entry_value(entry, (void **)&value);
    printf("{%s: %c} ", key, *value);
  }

  hash_table_iterator_reset(iterator);

  printf("\n");

  // de-allocate
  arena_destroy(&arena);

  return 0;
}
