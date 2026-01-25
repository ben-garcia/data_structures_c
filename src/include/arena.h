/*
 * @file arena.h

 * @brief Arena allocator implementation using demand paging.
 *
 * Reserve a large Virtual Memory Area but commit only what is required.
 *
 * @date 1/19/2026
 */

#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#ifndef ARENA_H
#define ARENA_H

/**
 * @brief Convert 's' in KB to bytes.
 *
 * @param s integer to convert
 */
#define KB(s) ((uint64_t)(s) << 10)

/**
 * @brief Convert 's' in MB to bytes.
 *
 * @param s integer to convert
 */
#define MB(s) ((uint64_t)(s) << 20)

/**
 * @brief Convert 's' in GB to bytes.
 *
 * @param s integer to convert
 */
#define GB(s) ((uint64_t)(s) << 30)

typedef struct arena {
  uint8_t *base_ptr;       // Start of the reservation
  uint64_t reserved_size;  // Total size (e.g. 1GB)
  uint64_t committed_size; // Currently commited memory
  uint64_t offset;         // Bump pointer
} arena;

typedef struct scratch_arena {
  arena *arena;                   // The to which to alloate from.
  const uint64_t original_offset; // The offset of the arena.
} scratch_arena;

/**
 * @brief Reserves the virtual Memory Area but doesn't commit any physical
 * memory yet.
 *
 * @param reserve_size size of the Virtual Memory Area.
 * @return arena
 */
arena *arena_create(size_t reserve_size);

/**
 * @brief Commit 'size' from the Virtual Memory Area.
 *
 * @param a arena to modify
 * @param size memory block size to commit.
 * @param alignment the alignment boundary.
 * @return pointer to the start of the newly committed memory.
 */
void *arena_alloc(arena *a, size_t size, size_t alignment);

/**
 * @brief Commit 'size' from the Virtual Memory Area.
 *
 * @param a arena to modify
 * @param new_size sizs of the new memory block.
 * @param old_ptr start of memory chuck to reallocate.
 * @return pointer to the start of the newly resized memory.
 */
void *arena_realloc(arena *a, uint8_t *old_ptr, size_t new_size);

/**
 * @brief reset the arena
 *
 * @param a arena to modify
 */
void arena_reset(arena *a);

/**
 * @brief Deallocate memory used
 *
 * @param a arena to de-allocate
 */
void arena_destroy(arena *a);

/**
 * @brief Create and setup the scrach_arena using 'a'
 *
 * @param a arena to use
 * @return scratch_arena
 */
scratch_arena scratch_arena_create(arena *a);

/**
 * @brief Allocate 'size' bytes.
 *
 * @param sa scratch_arena
 * @param size bytes to allocate
 * @param alignment the alignment boundary.
 */
void *scratch_arena_alloc(scratch_arena *sa, size_t size, uint64_t alignment);

/**
 *
 * @brief Reset the scratch_arena
 *
 * @param sa scratch_arena to reset
 */
void scratch_arena_destroy(scratch_arena *sa);

#endif // ARENA_H
