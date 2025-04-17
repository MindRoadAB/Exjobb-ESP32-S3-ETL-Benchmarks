#pragma once

#if USE_ETL
    #include <etl/unordered_set.h>
    #include <etl/string.h>
    #define MAX_SET_SIZE 48
    #define STRLN 4 
    using _set_int = etl::unordered_set<int, MAX_SET_SIZE>;
    using _set_string = etl::unordered_set<etl::string<STRLN>, MAX_SET_SIZE>;
#else
    #include <unordered_set>
    #include <string> 
    using _set_int = std::unordered_set<int>;
    using _set_string = std::unordered_set<std::string>;
#endif

extern _set_int uset_i1;
extern _set_int uset_i2;
extern _set_int uset_iempty;
extern _set_string uset_s1;
extern _set_string uset_s2;
extern _set_string uset_sempty;
