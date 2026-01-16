#ifndef SCC_MEMORY_H
#define SCC_MEMORY_H

#include <stdlib.h>

void *scc_malloc(size_t size);
void *scc_calloc(size_t nmemb, size_t size);
void *scc_realloc(void *ptr, size_t size);
void scc_free(void *ptr);

#endif // SCC_MEMORY_H
