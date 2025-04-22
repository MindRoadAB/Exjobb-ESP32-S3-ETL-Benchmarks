#pragma once
#include <string>

#if USE_ETL
    #include <etl/string.h>
    #include <etl/unordered_map.h>
    #include <etl/map.h> 
    #define STRLN 4
    #define MAX_MAP_SIZE 16
    
    using _string_sm = etl::string<STRLN>;
    using _map = etl::unordered_map<int, _string_sm, MAX_MAP_SIZE>;

#else
    #include <unordered_map>
    using _string = std::string;
    using _map = std::unordered_map<int, _string>;

#endif

extern _map umap_1;
extern _map umap_2;
extern _map umap_empty;
extern _map umap_large;

#if USE_ETL
    extern std::pair<int, _string_sm> mapElem;
#else
    extern std::pair<int, _string> mapElem;
#endif

void map_benchmark(uint32_t);





