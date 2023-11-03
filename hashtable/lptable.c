#include "lptable.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

struct lptable_entry {
  char *key;
  void *value;
};

struct lptable {
  struct lptable_entry *entries;
  size_t size;
  size_t capacity;
  size_t data_size;
  void (*freefn)(void *);
};

struct lptable_iter {
  struct lptable_entry *entries;
  size_t size;
  size_t capacity;
  int index;
};

#define LPTABLE_LOAD_FACTOR 0.75
#define LPTABLE_INITIAL_SIZE 8

lptable *lptable_create(size_t data_size) {
  lptable *table;

  if ((table = malloc(sizeof(*table))) == NULL) {
    return NULL;
  }

  table->data_size = data_size;
  table->size = 0;
  table->capacity = LPTABLE_INITIAL_SIZE;
  table->freefn = NULL;
  table->entries = NULL;

  return table;
}

/* FNV-1a */
static uint32_t hash(const char *key, int length) {
  uint32_t hash = 2166136261U;

  for (int i = 0; i < length; i++) {
    hash ^= (uint8_t)key[i];
    hash *= 16777619;
  }

  // unsigned int hash = 5381;
  //
  // while (length--)
  //   hash = ((hash << 5) + hash) + (*key++);
  return hash;
}

static lptable_entry *find_entry(lptable_entry *entries, int capacity,
                                 const char *key) {
  uint32_t index = hash(key, strlen(key)) & (capacity - 1);
  lptable_entry *tombstone = NULL;

  for (;;) {
    lptable_entry *entry = &entries[index];

    if (entry->key == NULL) {
      if (entry->value == NULL) {
        return tombstone != NULL ? tombstone : entry;
      } else {
        if (tombstone == NULL) {
          tombstone = entry;
        }
      }
    } else if (*key == *entry->key && strcmp(entry->key, key) == 0) {
      return entry;
    }

    index = (index + 1) & (capacity - 1);
  }
}

static void resize(lptable *table, int capacity) {
  lptable_entry *new_entries = malloc(capacity * sizeof(lptable_entry));

  for (int i = 0; i < capacity; i++) {
    new_entries[i].key = NULL;
    new_entries[i].value = NULL;
  }

  table->size = 0;

  for (int i = 0; i < table->capacity; i++) {
    lptable_entry *entry = &table->entries[i];

    if (entry->key == NULL) {
      continue;
    }

    lptable_entry *dest = find_entry(new_entries, capacity, entry->key);

    dest->key = entry->key;
    dest->value = entry->value;
    table->size++;
  }

  free(table->entries);

  table->entries = new_entries;
  table->capacity = capacity;
}

bool lptable_insert(lptable *table, const char *key, void *value) {
  if (table == NULL) {
    return false;
  }

  if (table->size == 0) {
    table->entries = malloc(table->capacity * sizeof(lptable_entry));

    for (int i = 0; i < table->capacity; i++) {
      table->entries[i].key = NULL;
      table->entries[i].value = NULL;
    }
  }

  if (table->size + 1 >= table->capacity * LPTABLE_LOAD_FACTOR) {
    resize(table, table->capacity * 2);
  }

  lptable_entry *entry = find_entry(table->entries, table->capacity, key);

  bool is_new_key = entry->key == NULL;

  if (!is_new_key) {
    return false;
  }

  if (entry->value != (void *)true) {
    table->size++;
  }

  entry->key = malloc(sizeof(char) * (strlen(key) + 1));
  strcpy(entry->key, key);

  entry->value = value;

  return is_new_key;
}

bool lptable_insert_and_replace(lptable *table, const char *key, void *value) {
  if (table == NULL) {
    return false;
  }

  if (table->size == 0) {
    table->entries = malloc(table->capacity * sizeof(lptable_entry));

    for (int i = 0; i < table->capacity; i++) {
      table->entries[i].key = NULL;
      table->entries[i].value = NULL;
    }
  }

  if (table->size + 1 >= table->capacity * LPTABLE_LOAD_FACTOR) {
    resize(table, table->capacity * 2);
  }

  lptable_entry *entry = find_entry(table->entries, table->capacity, key);

  bool is_new_key = entry->key == NULL;

  if (is_new_key && entry->value != (void *)true) {
    table->size++;
  }

  if (is_new_key) {
    entry->key = malloc(sizeof(char) * (strlen(key) + 1));
    strcpy(entry->key, key);
  } else {
    if (table->freefn != NULL) {
      table->freefn(entry->value);
    }
  }

  entry->value = value;

  return is_new_key;
}

void *lptable_find(lptable *table, const char *key) {
  if (table->size == 0) {
    return NULL;
  }

  lptable_entry *entry = find_entry(table->entries, table->capacity, key);

  if (entry->key == NULL) {
    return NULL;
  }

  return entry->value;
}

bool lptable_remove(lptable *table, const char *key) {
  if (table->size == 0) {
    return false;
  }

  lptable_entry *entry = find_entry(table->entries, table->capacity, key);

  if (entry->key == NULL) {
    return false;
  }

  free(entry->key);

  entry->key = NULL;
  entry->value = (void *)true;

  return true;
}

const char *lptable_get_entry_key(lptable_entry *entry) { return entry->key; }

void *lptable_get_entry_value(lptable_entry *entry) { return entry->value; }

void lptable_set_freefn(lptable *table, void (*freefn)(void *)) {
  table->freefn = freefn;
}

void lptable_destroy(lptable *table) {
  if (table == NULL) {
    return;
  }

  if (table->size == 0) {
    free(table);
    return;
  }

  for (int i = 0; i < table->capacity; i++) {
    if (table->entries[i].key != NULL && table->entries[i].value != NULL) {
      free(table->entries[i].key);

      if (table->freefn != NULL) {
        table->freefn(table->entries[i].value);
      }
    }
  }

  free(table->entries);
  free(table);
}

lptable_iter *lptable_iter_create(lptable *table) {
  lptable_iter *it;

  if ((it = malloc(sizeof(*it))) == NULL) {
    return NULL;
  }

  it->entries = table->entries;
  it->size = table->size;
  it->capacity = table->capacity;
  it->index = 0;

  return it;
}

lptable_entry *lptable_iter_next(lptable_iter *it) {
  if (it->size == 0) {
    return NULL;
  }

  if (it->index > it->capacity - 1) {
    return NULL;
  }

  for (int i = it->index; i <= it->capacity - 1; i++) {
    lptable_entry *entry = &it->entries[i];

    if (entry->key != NULL && entry->value != NULL) {
      it->index++;
      return entry;
    }

    it->index++;
  }

  return NULL;
}

void lptable_iter_reset(lptable_iter *it) { it->index = 0; }

void lptable_iter_destroy(lptable_iter *it) { free(it); }
