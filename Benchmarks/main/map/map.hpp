#pragma once


#if USE_ETL
    #include <etl/string.h>
    #include <string>    
    #include <etl/unordered_map.h>
    #include <etl/map.h> 
    #define STRLN 4;
    #define MAX_MAP_SIZE 16;

    using _map = etl::unordered_map<int, std::string, 16>;

#else
    #include <unordered_map>
    #include <string>
    using _string = std::string;
    using _map = std::unordered_map<int, _string>;

#endif

extern _map umap_1;
extern _map umap_2;
extern _map umap_empty;
extern _map umap_large;

