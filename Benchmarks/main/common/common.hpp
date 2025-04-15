#include <stdio.h>
#include "esp_timer.h"
#include "esp_cpu.h"



#define MEASUREMENTS 5000 

/** 
 *  Wrap the given expression in calls to esp_cpu_get_cycle_count() to measure the cycles.
 *  Default number of iterations is MEASUREMENTS. See CYCLE_GET_COUNT_CUSTOM_ITERS which takes
 *  a custom number of iterations.
 *  The expression is executed once before the measurement to account for any setup time.
 *  The expression is executed MEASUREMENTS times after the first execution to get a more
 *  accurate measurement.
 *  The time taken is printed in cycles per iteration with given label.
 */
#define CYCLE_GET_COUNT(expr, time, label)                                                  \
    do {                                                                                    \
        uint32_t __start = esp_cpu_get_cycle_count();                                       \
        expr;                                                                               \
        for (int retries = 0; retries < MEASUREMENTS; retries++) {                          \
            expr;                                                                           \
        }                                                                                   \
        time = esp_cpu_get_cycle_count() - __start;                                         \
        printf("%s operation Ran %u iterations, @ %lu cycles per iteration\n",              \
                label, MEASUREMENTS, (time / MEASUREMENTS));                                \
    } while (0)

/**
 * Wrap the given expression in calls to esp_cpu_get_cycle_count() to measure the cycles, 
 * with the given number of iterations.
 *  The expression is executed once before the measurement to account for any setup time.
 *  The expression is executed MEASUREMENTS times after the first execution to get a more
 *  accurate measurement.
 *  The time taken is printed in cycles per iteration with given label. 
 */
 #define CYCLE_GET_COUNT_CUSTOM_ITERS(expr, time, iters, label)                              \
    do {                                                                                    \
        uint32_t __start = esp_cpu_get_cycle_count();                                       \
        expr;                                                                               \
        for (int retries = 0; retries < iters; retries++) {                                 \
            expr;                                                                           \
        }                                                                                   \
        time = esp_cpu_get_cycle_count() - __start;                                         \
        printf("%s operation Ran %u iterations, @ %lu cycles per iteration\n",              \
                label, iters, (time / iters));                                              \
    } while (0)

/** 
 *  Wrap the given expression in calls to esp_cpu_get_cycle_count() to measure the cycles, for expressions
 *  that return a value, cast to void to avoid unused variable warnings.
 *  Default number of iterations is MEASUREMENTS. See CYCLE_GET_COUNT_CUSTOM_ITERS which takes
 *  a custom number of iterations.
 *  The expression is executed once before the measurement to account for any setup time.
 *  The expression is executed MEASUREMENTS times after the first execution to get a more
 *  accurate measurement.
 *  The time taken is printed in cycles per iteration with given label.
 */
#define CYCLE_GET_COUNT_VAL(expr, time, label)                                              \
    do {                                                                                    \
        (void)expr;                                                                         \
        uint32_t __start = esp_cpu_get_cycle_count();                                       \
        for (int retries = 0; retries < MEASUREMENTS; retries++) {                          \
            (void)expr;                                                                     \
        }                                                                                   \
        time = esp_cpu_get_cycle_count() - __start;                                         \
        printf("%s operation Ran %u iterations, @ %lu cycles per iteration\n",              \
                label, MEASUREMENTS, (time / MEASUREMENTS));                                \
    } while (0)


/**
 * Wrap the given expression in calls to esp_timer_get_time() to measure the elapsed time of the
 * operation in microseconds.
 */
#define TIME_TASK(expr, res, label)                                                         \
    do {                                                                                    \
        uint64_t start = esp_timer_get_time();                                              \
        expr;                                                                               \
        for (int retries = 0; retries < MEASUREMENTS; retries++) {                          \
            expr;                                                                           \
        }                                                                                   \
        uint64_t end = esp_timer_get_time();                                                \
        printf("%s operation; Ran %u iterations took %llu milliseconds                      \
            (%llu microseconds per invocation)\n",                                          \
            label, MEASUREMENTS, (end -start)/1000, (end - start)/MEASUREMENTS);            \
    } while (0) 


/**
 * Wrapper for testing construction operation.
 */
#define TEST_CONSTRUCTOR(expr, time, label)     \
    CYCLE_GET_COUNT(                            \
        expr,                                   \
        time,                                   \
        "constructor " label \
    );                  \

/** 
 * Wrapper for testing element access 
 */
#define TEST_ELEM_ACCESS(expr, time, label)     \
    CYCLE_GET_COUNT_VAL(                        \
        expr,                                   \
        time,                                   \
        "element access " label \
    );               \

    /**
 * Wrapper for testing reverse operation.
 */
#define TEST_REVERSE(expr, time, label)         \
    CYCLE_GET_COUNT(                            \
        (_reverse(expr.begin(), expr.end())),                                   \
        time,                                   \
        "reverse " label \
    );                      \

    /**
 * Wrapper for testing append (or operator+=) operation.
 */
#define TEST_APPEND(expr, time, iters, label)          \
    CYCLE_GET_COUNT_CUSTOM_ITERS(                            \
        expr,                                   \
        time,                                   \
        iters, \
        "append " label \
    );                       \

/**
 * Wrapper for testing comparison operation.
 * expr argument should be container.compare(other_container)
 */
#define TEST_COMPARE(expr1, expr2, time, label)         \
    CYCLE_GET_COUNT_VAL(                            \
        (expr1.compare(expr2)),                                   \
        time,                                   \
        "compare " label                        \
    );                      \

#define TEST_POP_BACK(expr1, time, label)         \
    CYCLE_GET_COUNT_VAL(                            \
        (expr1.pop_back()),                                   \
        time,                                   \
        "compare " label                        \
    );                      \








