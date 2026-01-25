#include "include/arena.h"
#include <string.h>

#define IS_POWER_OF_TWO(n) (((uint64_t)(n) & ((uint64_t)(n) - 1)) == 0)
/**
 * Aligns 'n' up to the nearest 'p'(power of 2).
 */
#define ALIGN_UP_POW2(n, p)                                                    \
  (((uint64_t)(n) + ((uint64_t)(p) - 1)) & (~((uint64_t)(p) - 1)))

/**
 * Get the size of a block of virtual memory from the OS.
 */
static size_t get_page_size() { return sysconf(_SC_PAGESIZE); }

/**
 * @brief
 *
 */
static void *align_forward(uint8_t *address, uint64_t alignment) {
  uintptr_t addr =
      (uintptr_t)address; // Convert to integer type for calculation

  // Ensure alignment is a power of 2
  // if (alignment == 0 || (alignment & (alignment - 1)) != 0) {
  if (alignment == 0 || !IS_POWER_OF_TWO(alignment)) {
    return NULL; // Invalid alignment
  }

  // Calculate the next aligned address
  uintptr_t aligned_addr = ALIGN_UP_POW2(addr, alignment);
  // uintptr_t aligned_addr = (addr + alignment - 1) & ~(alignment - 1);

  return (void *)aligned_addr; // Cast back to void pointer
}

arena *arena_create(size_t reserve_size) {
  arena *a = malloc(sizeof(arena));
  if (a == NULL) {
    return NULL;
  }

  const size_t page_size = get_page_size();

  // TODO: delete me
  uint64_t deleteme = ALIGN_UP_POW2(reserve_size, page_size);
  (void)deleteme;
  /// Align reservation up to the nearest page size
  // Align to a page boundary
  reserve_size = (reserve_size + page_size - 1) / page_size * page_size;

  // Reserve the Virtual Memory Area but does not alloate phsycial memory.
  void *block =
      mmap(NULL, reserve_size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (block == MAP_FAILED) {
    free(a);
    return NULL;
  }

  a->base_ptr = (uint8_t *)block;
  a->reserved_size = reserve_size;
  a->committed_size = 0;
  a->offset = 0;

  return a;
}

void *arena_alloc(arena *a, uint64_t size, uint64_t alignment) {
  if (a == NULL || size == 0) {
    return NULL;
  }

  uint64_t aligned_ptr =
      (uint64_t)align_forward(a->base_ptr + a->offset, alignment);
  (void)aligned_ptr;
  const size_t page_size = get_page_size();
  const size_t new_offset = a->offset + size;
  if (new_offset > a->reserved_size) {
    return NULL; // Out of reserved space
  }

  // check Virutal Memory Area has been commited.
  if (new_offset > a->committed_size) {
    uint64_t deleteme = ALIGN_UP_POW2(new_offset, page_size);
    (void)deleteme;
    // Align the required commit size up to nearest page
    size_t new_commit_target =
        (new_offset + page_size - 1) / page_size * page_size;
    // Clamp to the reservation limit
    if (new_commit_target > a->reserved_size) {
      new_commit_target = a->reserved_size;
    }

    const size_t size_to_commit = new_commit_target - a->committed_size;
    void *commit_start_addr = a->base_ptr + a->committed_size;

    // Allocate phsycial memory pages(4KB) to the reserved Virtual Memory Area.
    if (mprotect(commit_start_addr, size_to_commit, PROT_READ | PROT_WRITE) !=
        0) {
      return NULL;
    }

    a->committed_size = new_commit_target;
  }

  void *memory = a->base_ptr + a->offset;
  a->offset = new_offset;

  return memory;
}

void *arena_realloc(arena *a, uint8_t *old_ptr, size_t new_size) {
  if (new_size == 0 || new_size > a->reserved_size) {
    return NULL; // out of space
  }

  const size_t old_size = a->offset;
  void *memory = a->base_ptr + new_size; 

  memcpy(memory, old_ptr, old_size);

  return memory;
}

void arena_reset(arena *a) { a->offset = 0; }

void arena_destroy(arena *a) {
  munmap(a->base_ptr, a->reserved_size);
  free(a);
}

scratch_arena scratch_arena_create(arena *a) {
  return (scratch_arena){.arena = a, .original_offset = a->offset};
}

void *scratch_arena_alloc(scratch_arena *sa, size_t size, uint64_t alignment) {
  return arena_alloc((arena *)sa->arena, size, alignment);
}

void scratch_arena_destroy(scratch_arena *sa) {
  sa->arena->offset = sa->original_offset;
}
