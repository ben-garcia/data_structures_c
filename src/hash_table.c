#include "include/hash_table.h"
#include "include/utils.h"

#include <stdalign.h>
#include <string.h>
#include <stdio.h>

#define HASH_TABLE_LOAD_FACTOR 0.75
#define TRUE 1
#define FALSE 0

struct hash_table_entry {
  char *key;
  void *value;
};

struct hash_table {
  hash_table_entry *entries; // array of entries
  unsigned int (*hashfn)(const char *,
                         unsigned int); // function to generate hash code
  void (*freefn)(void **);              // function to deallocate each entry
  arena *arena;                         // memory block for allocations
  unsigned size;                        // number of entries
  unsigned int capacity;                // number of buckets
  unsigned int data_size;               // sizeo of each item in the buckets
};

struct hash_table_iterator {
  hash_table_entry *entries;
  unsigned size;
  unsigned capacity;
  unsigned int index;
};

/**
 * FNV-1A hashing function.
 *
 * source:
 * https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function`
 */
static unsigned int hash(const char *key, unsigned int length) {
  unsigned int hash_code = 2166136261U;

  for (unsigned int i = 0; i < length; i++) {
    // for FNV-1 switch the order below
    hash_code ^= (unsigned char)key[i];
    hash_code *= 16777619;
  }

  return hash_code;
}

/**
 * Retreive a hash table entry.
 *
 * Used in search/insert/delete operations.
 *
 * @param array of hash table entries.
 * @param capacity max number of entries the hash table can hold at this time.
 * @param key identifier used to search for.
 * @param key identifier used to search for.
 * @return hash table entry to modify.
 */
static hash_table_entry *
find_entry(hash_table_entry *entries, unsigned int capacity, const char *key,
           unsigned int (*hashfn)(const char *, unsigned int)) {
  unsigned int key_length = strlen(key);
  unsigned int index = hashfn(key, key_length) & (capacity - 1);
  hash_table_entry *tombstone = NULL;

  while (1) {
    hash_table_entry *entry = &entries[index];

    if (entry->key == NULL) {
      // Found empty entry
      if (entry->value == NULL) {
        return tombstone != NULL ? tombstone : entry;
      } else {
        // Found tombstone(prevously deleted entry).
        if (tombstone == NULL) {
          tombstone = entry;
        }
      }
    } else if (strncmp(entry->key, key, key_length) == 0) {
      return entry; // found collision
    }

    // increment by one, or wrap around to 0 index
    index = (index + 1) & (capacity - 1);
  }
}

/**
 * Resize the hash table after load factor has been reached/exceeded.
 *
 * @param ht hash table to modifiy.
 * @param capacity the new capacity.
 */
static void resize(hash_table *ht, int capacity) {
  hash_table_entry *new_entries =
      arena_alloc(ht->arena, capacity * sizeof(hash_table_entry),
                  alignof(hash_table_entry), FALSE);

  for (int i = 0; i < capacity; i++) {
    new_entries[i].key = NULL;
    new_entries[i].value = NULL;
  }

  ht->size = 0;

  for (unsigned i = 0; i < ht->capacity; i++) {
    hash_table_entry *entry = &ht->entries[i];

    if (entry->key == NULL) {
      continue;
    }

    hash_table_entry *dest =
        find_entry(new_entries, capacity, entry->key, ht->hashfn);

    dest->key = entry->key;
    dest->value = entry->value;
    ht->size++;
  }

  // free(ht->entries);

  ht->entries = new_entries;
  ht->capacity = capacity;
}

int hash_table_create(hash_table **ht, unsigned int initial_capacity,
                      unsigned int data_size,
                      unsigned int (*hashfn)(const char *, unsigned int),
                      void (*freefn)(void **), arena *arena) {
  ASSERT(arena != NULL, "arena MUST be provided");

  if ((*ht = arena_alloc(arena, sizeof(hash_table), alignof(hash_table),
                         FALSE)) == NULL) {
    return 1;
  }

  (*ht)->arena = arena;
  (*ht)->data_size = data_size;
  (*ht)->size = 0;
  (*ht)->capacity =
      (initial_capacity <= 0) ? 16 : initial_capacity; // initial capacity
  (*ht)->hashfn = hashfn == NULL ? hash : hashfn;
  (*ht)->freefn = freefn;
  (*ht)->entries = NULL;

  return 0;
}

int hash_table_get_size(hash_table *ht) {
  if (ht == NULL) {
    return -1;
  }

  return ht->size;
}

int hash_table_insert(hash_table *ht, const char *key, const void *value) {
  if (ht == NULL) {
    return 1;
  }

  if (ht->size == 0) {
    ht->entries =
        arena_alloc(ht->arena, ht->capacity * sizeof(hash_table_entry),
                    alignof(hash_table_entry), FALSE);

    // Setting keys and values to NULL indicates the position is empty. As
    // opposed to a tombstone.
    for (unsigned i = 0; i < ht->capacity; i++) {
      ht->entries[i].key = NULL;
      ht->entries[i].value = NULL;
    }
  }

  // Double the capacity of the ht when load factor is reached.
  if (ht->size + 1 >= ht->capacity * HASH_TABLE_LOAD_FACTOR) {
    resize(ht, ht->capacity * 2);
  }

  hash_table_entry *entry =
      find_entry(ht->entries, ht->capacity, key, ht->hashfn);
  int is_new_key = entry->key == 0;
  int key_length = strlen(key);

  if (!is_new_key) {
    // Key is already found
    return 1;
  }

  if (entry->value != (void *)1) {
    ht->size++;
  }

  entry->key = arena_alloc(ht->arena, sizeof(char) * (key_length + 1),
                           alignof(char), FALSE);
  strcpy(entry->key, key);
  entry->key[key_length] = '\0';

  if (ht->freefn == NULL) {
    entry->value =
        arena_alloc(ht->arena, ht->data_size, alignof(void *), FALSE);
    memcpy(entry->value, value, ht->data_size);
  } else {
    // Indicates user is responsible for allocate/deallocate memory.
    entry->value = (void *)value;
  }

  return 0;
}

int hash_table_insert_and_replace(hash_table *ht, const char *key,
                                  void *value) {
  if (ht == NULL) {
    return 1;
  }

  if (ht->size == 0) {
    ht->entries =
        arena_alloc(ht->arena, ht->capacity * sizeof(hash_table_entry),
                    alignof(hash_table_entry), FALSE);

    // Setting keys and values to NULL indicates the position is empty. As
    // opposed to a tombstone.
    for (unsigned int i = 0; i < ht->capacity; i++) {
      ht->entries[i].key = NULL;
      ht->entries[i].value = NULL;
    }
  }

  // Double the capacity of the ht when load factor is reached.
  if (ht->size + 1 >= ht->capacity * HASH_TABLE_LOAD_FACTOR) {
    resize(ht, ht->capacity * 2);
  }

  hash_table_entry *entry =
      find_entry(ht->entries, ht->capacity, key, ht->hashfn);
  int is_new_key = entry->key == NULL;
  int key_length = strlen(key);

  if (is_new_key) {
    ht->size++;

    entry->key = arena_alloc(ht->arena, sizeof(char) * (key_length + 1),
                             alignof(char), FALSE);
    strcpy(entry->key, key);
    entry->key[key_length] = '\0';

    if (ht->freefn == NULL) {
      entry->value =
          arena_alloc(ht->arena, ht->data_size, alignof(void *), FALSE);
      memcpy(entry->value, value, ht->data_size);
    } else {
      // Indicates user is responsible for allocate/deallocate memory.
      entry->value = value;
    }
  } else {
    if (ht->freefn == NULL) {
      memcpy(entry->value, value, ht->data_size);
    } else {
      ht->freefn(&entry->value);
      entry->value = value;
    }
  }

  return 0;
}

int hash_table_search(hash_table *ht, const char *key, void **value) {
  if (ht == NULL || ht->size == 0) {
    return 1;
  }

  if (strlen(key) == 0) {
    if (value != NULL) {
      // Ignore value
      *value = NULL;
    }
    return 1;
  }

  hash_table_entry *entry =
      find_entry(ht->entries, ht->capacity, key, ht->hashfn);

  if (entry->key == NULL) {
    if (value != NULL) {
      // Ignore value
      *value = NULL;
    }
    return 1;
  }

  if (value != NULL) {
    // Ignore value
    *value = entry->value;
  }

  return 0;
}

int hash_table_delete(hash_table *ht, const char *key) {
  if (ht == NULL || ht->size == 0) {
    return 1;
  }

  hash_table_entry *entry =
      find_entry(ht->entries, ht->capacity, key, ht->hashfn);

  // Key not found, there is no entry to delete.
  if (entry->key == NULL) {
    return 1;
  }

  ht->size--;

  entry->key = NULL;

  // entry value of 1 means the entry is a tombstone .
  entry->value = (void *)1;

  return 0;
}

int hash_table_get_entry_key(hash_table_entry *entry, char **key) {
  if (entry == NULL) {
    *key = NULL;
    return 1;
  }

  *key = entry->key;
  return 0;
}

int hash_table_get_entry_value(hash_table_entry *entry, void **value) {
  if (entry == NULL) {
    *value = NULL;
    return 1;
  }

  *value = entry->value;
  return 0;
}

int hash_table_destroy(hash_table **ht) {
  if (*ht == NULL) {
    // Deallocation only occurs for a previously created hash table.
    // When the hash table is empty.
    if ((*ht)->size == 0) {
      return 1;
    }
  }

  for (unsigned int i = 0; i < (*ht)->capacity; i++) {
    if ((*ht)->entries[i].key != NULL && (*ht)->entries[i].value != NULL) {

      if ((*ht)->freefn != NULL) {
        (*ht)->freefn(&(*ht)->entries[i].value);
      }
    }
  }

  return 0;
}

int hash_table_iterator_create(hash_table_iterator **it, hash_table *ht) {
  if (ht == NULL || ht->size == 0) {
    return 1;
  }

  ASSERT(ht->arena != NULL, "arena MUST be provided");

  if (((*it) = arena_alloc(ht->arena, sizeof(hash_table_iterator),
                           alignof(hash_table_iterator), FALSE)) == NULL) {
    return 1;
  }

  (*it)->entries = ht->entries;
  (*it)->size = ht->size;
  (*it)->capacity = ht->capacity;
  (*it)->index = 0;

  return 0;
}

int hash_table_iterator_next(hash_table_iterator *it,
                             hash_table_entry **entry) {
  if (it == NULL || it->size == 0 || it->index > it->capacity - 1) {
    return 1;
  }

  for (unsigned int i = it->index; i <= it->capacity - 1; i++) {
    *entry = &it->entries[i];

    if ((*entry)->key != NULL && (*entry)->value != NULL) {
      it->index++;
      break;
    }

    // If there is no entry at the last index there is no need to print.
    if (it->index >= it->capacity - 1) {
      return 1;
    }

    it->index++;
  }

  return 0;
}

int hash_table_iterator_reset(hash_table_iterator *it) {
  if (it == NULL) {
    return 1;
  }

  it->index = 0;

  return 0;
}
