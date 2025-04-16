#pragma once

#define STRLN 4;

#if USE_ETL
    #include <etl/unordered_map.h>
    #include <etl/string.h>
    using _u_map_int_string = etl::unordered_map<int, etl::string<STRLN>>; 
#else
    #include <unordered_map>
    #include <string>
    using _string = std::string;
    using _u_map_int_string = std::unordered_map<int, _string>;
#endif


extern _u_map_int_string umap_1;
extern _u_map_int_string umap_empty;
