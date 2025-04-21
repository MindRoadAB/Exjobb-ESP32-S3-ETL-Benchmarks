#include <stdio.h>
#include <numeric>
#include <array>

#include "esp_timer.h"
#include "esp_cpu.h"

#define MEASUREMENTS 5000
// std::array<uint32_t, 128> times{};
// uint32_t sum{};
// int int_placeholder{};

// #if USE_ETL
//     #include <etl/string.h>
//     etl::string<4> str_placeholder{};
// #else
//     #include <string>
//     std::string str_placeholder{};
// #endif
// For operations that do not mutate state


#define CYCLE_GET_COUNT(op_expr, time, label)                                       \
    do {                                                                        \
        op_expr;                                                       \
        uint32_t __start = esp_cpu_get_cycle_count();                             \
        for (int __i = 0; __i < MEASUREMENTS; ++__i) {                            \
            op_expr;                                                              \
        }                                                                         \
        time = esp_cpu_get_cycle_count() - __start;                               \
        printf("%s: %lu cycles/iteration\n", label, (time / MEASUREMENTS));      \
    } while (0)

// For mutating operations where cleanup is needed after each iteration
#define CYCLE_GET_COUNT_MUTATE(setup, op_expr, cleanup_expr, time, label) \
    do {                                                                          \
        setup;                                                                    \
        time = 0;                                                                 \
        for (int __i = 0; __i < MEASUREMENTS; ++__i) {                            \
            uint32_t __start = esp_cpu_get_cycle_count();                         \
            op_expr;                                                              \
            uint32_t __end = esp_cpu_get_cycle_count();                           \
            time += (__end - __start);                                            \
            cleanup_expr;                                                         \
        }                                                                         \
        printf("%s: %lu cycles/iteration\n", label, (time / MEASUREMENTS));      \
    } while (0)

// For ops that return a value, discard to avoid optimizing out
#define CYCLE_GET_COUNT_RETURN(op_expr, time, label)                                   \
    do {                                                                          \
        uint32_t __start = esp_cpu_get_cycle_count();                             \
        for (int __i = 0; __i < MEASUREMENTS; ++__i) {                            \
            (void)op_expr;                                                     \
        }                                                                         \
        time = esp_cpu_get_cycle_count() - __start;                               \
        printf("%s: %lu cycles/iteration\n", label, (time / MEASUREMENTS));      \
    } while (0)     
