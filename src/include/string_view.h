#ifndef STRING_VIEW_H
#define STRING_VIEW_H

#include <string.h>

typedef struct string_view {
  const char *data;
  unsigned int length;
} string_view;

/**
 * Create a string view to te source string. 
 *
 * @param view string_view to create.
 * @param source string to view.
 * @return 0 on success, 1 otherwise
 */
string_view string_view_create(const char *source);

/**
 * @brief Seperate string into different regions determined by a delimioter.
 *
 * @param view string_view to modify.
 * @param output where to store different regions.
 * @param delimiter character to mark the seperation.
 * @return 0 on succeview, 2 indicates memory allocation failed.
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
int string_view_compare(string_view first, string_view second);

#endif // STRING_VIEW_H
