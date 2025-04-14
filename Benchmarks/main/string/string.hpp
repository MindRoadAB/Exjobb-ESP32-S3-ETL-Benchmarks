#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#if USE_ETL
    #include <etl/string.h>
    #define MAX_STRLN 256 
    using _string = etl::string<MAX_STRLN>;
#else
    #include <string>
    using _string = std::string;
#endif



void string_constructor(void);
void task_string(void *);

