#ifndef STRING_VIEW_H
#define STRING_VIEW_H

#include "arena.h"
#include <string.h>

typedef struct string_view string_view;

/**
 * Create a string view to te source string.
 *
 * @param view string_view to create.
 * @param source string to view.
 * @param arena memory block for allcations 
 * @return 0 on success, 1 otherwise
 */
int string_view_create(string_view **view, const char *source, arena *arena);

/**
 * @brief Seperate string into different regions determined by a delimioter.
 *
 * @param view string_view to modify.
 * @param output where to store different regions.
 * @param delimiter character to mark the seperation.
 * @return 0 on success, 1 otherwise 
 *
 * @example
 *   ...
 *   string_view view = ...
 *   string_view output = ...
 *   while (string_view_split(view, output, ':') == 0) {
 *     // Do Work
 *   }
 *
 */
int string_view_split(string_view *view, string_view *output,
                      const char delimiter);

/**
 * @brief Remove whitespace before and after the string being built.
 *
 * @param view string_view to modify.
 * @return 0 on success, 1 otherwise
 */
int string_view_trim(string_view *view);

/**
 * @brief Use the string view to built a string.
 *
 * @param view string_view to modify.
 * @param buffer where to store the constructed string slice.
 * @return 0 on success, 1 otherwise
 *
 * @note caller must free memory on buffer
 */
int string_view_to_string(string_view *view, char **buffer);

/**
 * @brief Checks whether the string_view starts with the given prefix.
 *
 * @param view string_view to check.
 * @param prefix string to check against
 * @return 0 on success, 1 otherwise
 */

int string_view_starts_with(string_view *view, const char *prefix);

/**
 * @brief Checks whether the string_view ends with the given suffix.
 *
 * @param view string_view to modify.
 * @param suffix string to check against
 * @return 0 on success, 1 otherwise
 */
int string_view_ends_with(string_view *view, const char *suffix);

/**
 * @brief Compare first and second for equality
 *
 * @param first string_view
 * @param second string_view
 * @return 0 on success, 1 otherwise
 */
int string_view_compare(string_view *first, string_view *second);

/**
  * @brief Return the size of the 'view' 
  *
  * @param view the string view to check
  * @return 'view' size, -1 otherwise
  */
int string_view_size(const string_view *view);

/**
  * @brief Return a reference to the underline string of the 'view' 
  *
  * @param view the string view to check
  * @return reference to string held by the 'view', NULL otherwise
  */
char *string_view_data(const string_view *view);

#endif // STRING_VIEW_H
