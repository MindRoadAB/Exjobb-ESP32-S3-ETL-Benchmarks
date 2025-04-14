#include "common/common.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string/string.hpp"

#define STACK_DEPTH 4096
#define PRIORITY_DEFAULT 5
#define CORE_0 0
StaticTask_t xTaskBuffer;
StackType_t xStack[STACK_DEPTH];



extern "C" void app_main(void)
{
    #if USE_ETL
        printf("Using ETL...\n");
    #else
        printf("Using libstdc++...\n"); 
    #endif

    xTaskCreatePinnedToCore(task_string, "String Task", STACK_DEPTH, nullptr, PRIORITY_DEFAULT, nullptr, CORE_0);
    // xTaskCreateStaticPinnedToCore(
    //     task_string, 
    //     "static pinned to core string ops",
    //     2*STACK_DEPTH,
    //     NULL, 
    //     5, 
    //     xStack, 
    //     &xTaskBuffer, 
    //     0
    // );
}