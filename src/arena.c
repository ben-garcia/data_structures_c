#include "include/arena.h"
#include <string.h>

#define IS_POWER_OF_TWO(n) ((uint64_t)(n) & ((uint64_t)(n) - 1))
/**
 * Aligns 'n' up to the nearest 'p'(power of 2).
 */
#define ALIGN_UP_POW2(n, p)                                                    \
  (((uint64_t)(n) + ((uint64_t)(p) - 1)) & (~((uint64_t)(p) - 1)))

struct arena {
  uint8_t *base_ptr;       // pointer to the start of the reserved size
  uint64_t reserved_size;  // max size of the block of memory
  uint64_t committed_size; // size of physical memory
  uint64_t offset;         // bump pointer
  uint64_t scratch_offset; // store the offset for scratch arena
};

/**
 * Get the size of a block of virtual memory from the OS.
 */
static long get_page_size() { return sysconf(_SC_PAGESIZE); }

int arena_create(arena **a, uint64_t reserve_size) {
  if (((*a) = malloc(sizeof(arena))) == NULL) {
    return 1;
  }

  const uint32_t page_size = get_page_size();

  /// Align reservation up to the nearest page size
  // Align to a page boundary
  reserve_size = ALIGN_UP_POW2(reserve_size, page_size);

  // Reserve the Virtual Memory Area but does not alloate phsycial memory.
  void *block;
  if ((block = mmap(NULL, reserve_size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS,
                    -1, 0)) == MAP_FAILED) {
    free(*a);
    return 1;
  }

  (*a)->base_ptr = (uint8_t *)block;
  (*a)->reserved_size = reserve_size;
  (*a)->committed_size = 0;
  (*a)->offset = 0;
  (*a)->scratch_offset = 0;

  return 0;
}

void *arena_alloc(arena *arena, uint64_t size, uint64_t alignment,
                  unsigned int zero_out) {
  if (arena == NULL || size <= 0 || IS_POWER_OF_TWO(alignment) != 0) {
    return NULL;
  }

  const uint32_t page_size = get_page_size();
  const uint64_t new_offset = arena->offset + size;
  if (new_offset > arena->reserved_size) {
    return NULL; // Out of reserved space
  }

  // check Virutal Memory Area has been commited.
  if (new_offset > arena->committed_size) {
    // Align the required commit size up to nearest page
    uint64_t new_commit_target = ALIGN_UP_POW2(new_offset, page_size);
    // Clamp to the reservation limit
    if (new_commit_target > arena->reserved_size) {
      new_commit_target = arena->reserved_size;
    }

    const uint64_t size_to_commit = new_commit_target - arena->committed_size;
    void *commit_start_addr = arena->base_ptr + arena->committed_size;

    // Allocate phsycial memory pages(4KB) to the reserved Virtual Memory Area.
    if (mprotect(commit_start_addr, size_to_commit, PROT_READ | PROT_WRITE) !=
        0) {
      return NULL;
    }

    arena->committed_size = new_commit_target;
  }

  void *memory = arena->base_ptr + arena->offset;
  arena->offset = new_offset;

  if (zero_out == 1) {
    memset(memory, 0, size);
  }

  return memory;
}

void *arena_realloc(arena *arena, void *old_ptr, const uint64_t old_size,
                    const uint64_t new_size, unsigned int zero_out) {
  if ((new_size == 0) || ((arena->offset + new_size) > arena->reserved_size)) {
    return NULL; // out of space
  }

  void *memory;
  if ((memory = arena_alloc(arena, new_size, sizeof(void *), 0)) == NULL) {
    return NULL;
  }
  
  memcpy(memory, old_ptr, old_size); // 

  if (zero_out == 1) {
    // zero out memory past old data
    memset(memory + old_size, 0, new_size - old_size);
  }

  return memory;
}

int arena_start_scratch_arena(arena *a) {
  if (a == NULL || a->committed_size == 0) {
    return 1;
  }

  a->scratch_offset = a->offset;

  return 0;
}

int arena_end_scratch_arena(arena *a) {
  if (a == NULL || a->scratch_offset == 0) {
    return 1;
  }

  a->offset = a->scratch_offset;

  return 0;
}

int arena_reset(arena *a) {
  if (a == NULL) {
    return 1;
  }

  a->offset = 0;
  return 0;
}

int arena_destroy(arena **a) {
  if (*a == NULL) {
    return 1;
  }

  munmap((*a)->base_ptr, (*a)->reserved_size);
  free(*a);

  *a = NULL;

  return 0;
}
