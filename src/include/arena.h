/*
 * @file arena.h

 * @brief Arena allocator implementation using demand paging.
 *
 * Reserve a large Virtual Memory Area but commit only what is required.
 *
 * @date 1/19/2026
 */

#ifndef ARENA_H
#define ARENA_H

#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

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

typedef struct arena arena;

/**
 * @brief Reserves the virtual Memory Area but doesn't commit any physical
 * memory yet.
 *
 * @param a the arena to modify
 * @param reserve_size size of the Virtual Memory Area.
 * @return 0 on success, 1 otherwise
 */
int arena_create(arena **a, size_t reserve_size);

/**
 * @brief Commit 'size' from the Virtual Memory Area.
 *
 * @param arena the arena to modify
 * @param size memory block size to commit.
 * @param alignment the alignment boundary.
 * @param zero_out indicates whether to initialize the memory block
 * @return pointer to the start of the newly committed memory.
 */
void *arena_alloc(arena *arena, size_t size, size_t alignment,
                  unsigned int zero_out);

/**
 * @brief Commit 'size' from the Virtual Memory Area.
 *
 * @param a arena to modify
 * @param old_ptr start of memory chuck to reallocate
 * @param old_size size of the old memory block
 * @param new_size size of the new memory block
 * @param zero_out indicates whether to initialize the memory block
 * @return pointer to the start of the newly resized memory block
 */
void *arena_realloc(arena *a, void *old_ptr, const size_t old_size,
                    const size_t new_size, unsigned int zero_out);

/**
 * @brief Indicate the next allocations are temporary
 *
 * @param a arena to modify
 * @return 0 on success, 1 otherwise
 */
int arena_start_scratch_arena(arena *a);

/**
 * @brief Indicates the end of temporary allocations
 *
 * Used to reset the arena after scratch arena is no longer needed
 *
 * @param a arena to modify
 * @return 0 on success, 1 otherwise
 */
int arena_end_scratch_arena(arena *a);

/**
 * @brief reset the arena
 *
 * @param a arena to modify
 * @return 0 on success, 1 otherwise
 */
int arena_reset(arena *a);

/**
 * @brief Deallocate memory used
 *
 * @param a arena to de-allocate
 * @return 0 on success, 1 otherwise
 */
int arena_destroy(arena **a);

#endif // ARENA_H
