#include <stdio.h>
#include "esp_timer.h"
#include "esp_cpu.h"



#define MEASUREMENTS 5000 

#define CYCLE_GET_COUNT_VAL(expr, time, label)               \
    do {                                                \
        uint32_t __start = esp_cpu_get_cycle_count();   \
        auto res = expr; \
        for (int retries = 0; retries < MEASUREMENTS; retries++) {\
            res = expr;                                           \
        }\
        time = esp_cpu_get_cycle_count() - __start;      \
        printf("%s operation Ran %u iterations, @ %lu cycles per iteration, result is: %u\n", \
                label, MEASUREMENTS, (time / MEASUREMENTS), res); \
    } while (0)


#define CYCLE_GET_COUNT(expr, time, label)               \
    do {                                                \
        uint32_t __start = esp_cpu_get_cycle_count();   \
        expr; \
        for (int retries = 0; retries < MEASUREMENTS; retries++) {\
            expr;                                           \
        }\
        time = esp_cpu_get_cycle_count() - __start;      \
        printf("%s operation Ran %u iterations, @ %lu cycles per iteration\n", \
                label, MEASUREMENTS, (time / MEASUREMENTS)); \
    } while (0)


#define TIME_TASK(expr, res, label)                                                                \
    do {                                                                                    \
        uint64_t start = esp_timer_get_time();                                              \
        expr; \
        for (int retries = 0; retries < MEASUREMENTS; retries++) {                          \
            expr;                                                                           \
        }                                                                                   \
        uint64_t end = esp_timer_get_time();                                                \
        printf("%s operation; Ran %u iterations took %llu milliseconds (%llu microseconds per invocation)\n", \
            label, MEASUREMENTS, (end -start)/1000, (end - start)/MEASUREMENTS);                   \
    } while (0) 
 