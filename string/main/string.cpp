#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <ctime>
#include <experimental/random>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_cpu.h"
#include "esp_timer.h"
#include "esp_heap_caps.h"

#define MEASUREMENTS 100


#define CYCLE_GET_COUNT(expr, res)                      \
    do {                                                \
        uint32_t __start = esp_cpu_get_cycle_count();   \
        expr;                                           \
        res = esp_cpu_get_cycle_count() - __start;      \
    } while (0)


#define TIME_TASK(expr, res)                            \
    do {                                                \
        uint64_t start = esp_timer_get_time(); \
        for (int retries = 0; retries < MEASUREMENTS; retries++) { \
            expr;   \
        }                                       \
        uint64_t end = esp_timer_get_time(); \
        printf("%u iterations took %llu milliseconds (%llu microseconds per invocation)\n", \ 
            MEASUREMENTS, (end -start)/1000, (end - start)/MEASUREMENTS);   \
    } while (0)                                                              

StaticTask_t xTaskBuffer;
StackType_t xStack[ 4096 ];

uint64_t time_count;

const char *c_str_array[] = 

{
    "abcdefghijklmnopqrstuvwxyz",
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
    "abcdefghijklmnopqrstuvwxyzåäö",
    "ABCDEFGHIJKLMNOPQRSTUVWXYZÅÄÖ",
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ",
    "abcdefghijklmnopqrstuvwxyzåäöABCDEFGHIJKLMNOPQRSTUVWXYZÅÄÖ",
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzåäö",
    "ABCDEFGHIJKLMNOPQRSTUVWXYZÅÄÖabcdefghijklmnopqrstuvwxyz",
    "0123456789"
};

constexpr size_t num_strings = sizeof(c_str_array) / sizeof(c_str_array[0]);

#if USE_ETL
    #include <etl/string.h>
    #define MAX_STRLN 60 
    using _string = etl::string<MAX_STRLN>;
    _string str_array[num_strings]{};
#else
    #include <string>
    using _string = std::string;
    _string str_array[num_strings]{};
#endif

void 
dump_heap(const char *label)
{
    printf("[%s] Free heap: %u bytes\n", label, heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
}

void 
string_operations(void)
{
    dump_heap("START");


    

    for (size_t i = 0; i < num_strings; ++i)
    {
        str_array[i] = c_str_array[i];
        printf("%s\n", str_array[i].c_str());
        str_array[i].append(c_str_array[num_strings - 1]);
        printf("%s\n", str_array[i].c_str());
    
    }  
    
    _string str, str2 = {""};
    
    char i{65};
    while (i < 123) 
    {
        str += i;
        i++;
    }

    printf("str is %s\n", str.c_str());
    
    esp_cpu_cycle_count_t count = esp_cpu_get_cycle_count();
    std::reverse(str.begin(), str.end());
    esp_cpu_cycle_count_t reverse_count = esp_cpu_get_cycle_count() - count;
    printf("cycle count for reverse is: %lu\n", reverse_count);

    count = esp_cpu_get_cycle_count();
    _string s = {
                    "Hello my name is warren and I am 39 and I live in Sweden.\n"
                    "Hej Jag heter warren och jag är 39 och bor i Sverige."
                }; 
    reverse_count = esp_cpu_get_cycle_count() - count;
    printf("cycle count for constructor is: %lu\n", reverse_count);
    

    printf("str is now: %s\n", str.c_str());
    
   

    std::reverse(str.begin(), str.end());
    printf("str is now: %s\n", str.c_str());

    str2.resize(str.size());
    CYCLE_GET_COUNT(std::reverse_copy(std::begin(str), std::end(str), std::begin(str2)), reverse_count);
    printf("reverse copy cycle count: %lu\n", reverse_count);
                
    TIME_TASK(std::reverse_copy(std::begin(str), std::end(str), std::begin(str2)), time_count);
    
    printf("str2 is: %s\n", str2.c_str());
    dump_heap("END");

    i = 65;
    str = "";
    
    for (; i < 124; i = std::experimental::randint(65, 123))
    {
        if (123 == i)
        {
            str += '\0';
            break;
        }
        str += i;
    }

    printf("str is: %s\n", str.c_str());
}


void 
task_string_operations(void *arg)
{
    /** Do a warmup run */ 
    string_operations();
    
    size_t times{0};
    while (times != 1000)
    {
        printf("\nNow inside the loop at iteration %u\n", times); 
        string_operations();
        times++;
    }
    vTaskDelete(NULL);
}

void
task_heap_monitor(void *arg)
{
    
}


extern "C" void app_main(void)
{
#if USE_ETL
    printf("Using ETL...\n");
#else
    printf("Using libstdc++...\n"); 
#endif


    xTaskCreateStaticPinnedToCore(
        task_string_operations, 
        "static pinned to core string ops",
        4096,
        NULL, 
        5, 
        xStack, 
        &xTaskBuffer, 
        0
    );
    
    //xTaskCreatePinnedToCore(task_string_operations, "String operations", 4096, NULL, 5, NULL, 0);

}
