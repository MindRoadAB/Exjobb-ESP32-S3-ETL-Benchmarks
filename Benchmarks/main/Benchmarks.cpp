#include "common/common.hpp"

#if USE_STRING
    #include "string/string.hpp"
#elif USE_VECTOR
    #include "vector/vector.hpp"
#elif USE_MAP
    #include "map/map.hpp"
#elif USE_SET
    #include "set/set.hpp"
#elif USE_DEQUE
    #include "deque/deque.hpp"
#endif

int32_t cycles{0};

extern "C" void app_main(void)
{
    #if USE_ETL
        printf("Using ETL...\n");
    #else
        printf("Using libstdc++...\n"); 
    #endif

    #if USE_STRING
        printf("Benchmarking string operations...\n"); 
        string_benchmark(cycles);
    #elif USE_VECTOR
        printf("Benchmarking vector operations...\n"); 
        vector_benchmark(cycles); 
    #elif USE_MAP
        printf("Benchmarking unordered_map operations...\n"); 
        map_benchmark(cycles); 
    #elif USE_SET
        printf("Benchmarking unordered_set operations...\n"); 
        set_benchmark(cycles); 
    #elif USE_DEQUE     
        printf("Benchmarking deque operations...\n"); 
        deque_benchmark(cycles); 
    #endif
}

