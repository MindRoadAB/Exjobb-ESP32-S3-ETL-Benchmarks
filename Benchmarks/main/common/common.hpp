#pragma once

#include <stdio.h>


#include "esp_cpu.h"
#include "esp_log.h"

#define MEASUREMENTS 5000

#define CYCLE_GET_COUNT(op_expr, time, label)                                    \
    do {                                                                         \
        op_expr;                                                                 \
        uint32_t __start = esp_cpu_get_cycle_count();                            \
        for (int __i = 0; __i < MEASUREMENTS; ++__i) {                           \
            op_expr;                                                             \
        }                                                                        \
        time = esp_cpu_get_cycle_count() - __start;                              \
        printf("TEST: %s: %lu cycles/iteration\n", label, (time / MEASUREMENTS));      \
    } while (0)

// For mutating operations where cleanup is needed after each iteration
#define CYCLE_GET_COUNT_MUTATE(setup, op_expr, cleanup_expr, time, label)        \
    do {                                                                         \
        setup;                                                                   \
        op_expr;                                                                 \
        cleanup_expr;                                                            \
        time = 0;                                                                \
        for (int __i = 0; __i < MEASUREMENTS; ++__i) {                           \
            uint32_t __start = esp_cpu_get_cycle_count();                        \
            op_expr;                                                             \
            uint32_t __end = esp_cpu_get_cycle_count();                          \
            time += (__end - __start);                                           \
            cleanup_expr;                                                        \
        }                                                                        \
        printf("TEST: %s: %lu cycles/iteration\n", label, (time / MEASUREMENTS));      \
    } while (0)

// For ops that return a value, discard to avoid optimizing out
#define CYCLE_GET_COUNT_RETURN(op_expr, time, label)                             \
    do {                                                                         \
        (void)op_expr;                                                           \
        uint32_t __start = esp_cpu_get_cycle_count();                            \
        for (int __i = 0; __i < MEASUREMENTS; ++__i) {                           \
            (void)op_expr;                                                       \
        }                                                                        \
        time = esp_cpu_get_cycle_count() - __start;                              \
        printf("TEST: %s: %lu cycles/iteration\n", label, (time / MEASUREMENTS));      \
    } while (0)    

#if USE_ETL
    #include <etl/algorithm.h>
#else
    #include <algorithm>
#endif

template <typename Iter>
inline void _reverse(Iter first, Iter last) 
{
    #if USE_ETL
        etl::reverse(first, last);
    #else
        std::reverse(first, last);
    #endif
}
