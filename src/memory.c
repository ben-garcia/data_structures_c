#include "include/memory.h"

void *scc_malloc(size_t size) {
  void *pointer;
  if ((pointer = malloc(size)) == NULL) {
    return NULL;
  }

  return pointer;
}

void *scc_calloc(size_t nmemb, size_t size) {
  void *pointer;
  if ((pointer = calloc(nmemb, size)) == NULL) {
    return NULL;
  }

  return pointer;
}

void *scc_realloc(void *ptr, size_t size) {
  void *pointer;
  if ((pointer = realloc(ptr, size)) == NULL) {
    return NULL;
  }

  return pointer;
}

void scc_free(void *ptr) {
  if (ptr == NULL) {
    return;
  }

  free(ptr);
  ptr = NULL;
}

