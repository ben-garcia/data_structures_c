#ifndef STRING_BUILDER_H
#define STRING_BUILDER_H

#include "arena.h"
#include "string_view.h"

/*
 * @brief Dynamic string implemented using dynamic array.
 *
 * Creates a string character by character.
 */
typedef struct string_builder string_builder;

/**
 * Allocate necessary resources and setup.
 *
 * @param sb string_builder to create.
 * @param arena memory block for allocations 
 * @return 0 on success, 1 otherwise
 */
int string_builder_create(string_builder **sb, arena *arena);

/**
 * @brief Append a string to the string builder.
 *
 * @param sb string_builder to modify.
 * @param str string to add.
 * @return 0 on success, 1 otherwise
 */
int string_builder_append(string_builder *sb, const char *str);

/**
 * @brief Append a character to the string builder.
 *
 * @param sb string_builder to modify.
 * @param ch character to add.
 * @return 0 on success, 1 otherwise
 */
int string_builder_append_char(string_builder *sb, const char ch);

/**
 * @brief Append a formatted string to the string builder.
 *
 * @param sb string_builder to modify.
 * @param format formatted string.
 * @param ... variable number of arguments.
 * @return 0 on success, 1 otherwise
 */
int string_builder_append_fmt_str(string_builder *sb,
                                           const char *format, ...);

/**
 * @brief Append a string view to the string builder
 *
 * @param sb string_builder to modify.
 * @param view string_view to append
 * @return 0 on success, 1 otherwise
 */
int string_builder_append_view(string_builder *sb, const string_view *view);

/**
 * @brief Build string.
 *
 * @param sb string_builder to access.
 * @param buffer where to store the newly created string.
 * @return 0 on success, 1 otherwise
 */
int string_builder_build(string_builder *sb, char **buffer);

/**
 * @brief Check if string_builder is empty.
 *
 * @param sb string_builder to check.
 * @return 0 on success, 1 otherwise.
 */
int string_builder_is_empty(string_builder *sb);

#endif // STRING_BUILDER_H
