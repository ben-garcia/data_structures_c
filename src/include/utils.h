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

/**
 * Return power of two, greater than 'n' or 'n' if itself is a power of 2.
 * Use compound statement ({}) to use assignments
 *
 * @note add 'temp |= temp >> 32;' for 64bit usage
 *
 * @param n integer to check
 */
#define ROUND_POW2(n)                                                          \
  ((n) <= 0 ? 1 : ({                                                           \
    int temp = (n) - 1;                                                        \
    temp |= temp >> 1;                                                         \
    temp |= temp >> 2;                                                         \
    temp |= temp >> 4;                                                         \
    temp |= temp >> 8;                                                         \
    temp |= temp >> 16;                                                        \
    temp + 1;                                                                  \
  }))

#endif // UTILS_H
