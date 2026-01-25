/**
 * @file utils.h
 *
 * Helper macros
 *
 * @date 1/17/2026
 */

#ifndef UTILS_H
#define UTILS_H

#define CODE_ERROR "\033[31m"
#define CODE_RESET "\033[0m"

/**
 * @brief Think of assert() with a custom error message.
 *
 * Can helps find bugs in the code.
 *
 * @param condition the condition to evalutate against
 * @param message the error message
 */
#define ASSERT(condition, message)                                             \
  do {                                                                         \
    if (!(condition)) {                                                        \
      fprintf(stderr,                                                          \
              CODE_ERROR "fatal error" CODE_RESET ":%s:%i: '%s()': %s\n",      \
              __FILE__, __LINE__, __func__, message);                          \
      abort();                                                                 \
    }                                                                          \
  } while (0)

/**
 * @brief Print an error message and stops program execution
 *
 * @param message the error message to print
 */
#define PANIC(message)                                                         \
  do {                                                                         \
    fprintf(stderr,                                                            \
            CODE_ERROR "fatal error" CODE_RESET ":%s:%i: '%s()': %s\n",        \
            __FILE__, __LINE__, __func__, message);                            \
    abort();                                                                   \
  } while (0)

#endif // UTILS_H
