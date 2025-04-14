#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <algorithm>

#if USE_ETL
    #include <etl/string.h>
    #define MAX_STRLN 256 
    using _string = etl::string<MAX_STRLN>;
#else
    #include <string>
    using _string = std::string;
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

void string_constructor(void);
void string_elem_access(void);
void string_reverse(void);
void string_reverse_copy(void);
void string_append(void);

void task_string(void *);

