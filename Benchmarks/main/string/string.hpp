#pragma once

#include "../common/common.hpp"

#include <string.h>
#include <cstring>

#define MAX_STRLN 256 

#if USE_ETL
    #include <etl/string.h>
    #include <etl/string_view.h>
    using _string = etl::string<MAX_STRLN>;
    using _string_view = etl::string_view;
#else
    #include <string>
    #include <string_view>
    using _string = std::string;
    using _string_view = std::string_view;
#endif

extern const char *c_str_tiny;
extern const char *c_str_medium;
extern const char *c_str_large; 

extern const char *c_str_jumbo; 
extern _string _str_tiny;
extern _string _str_medium;
extern _string _str_large;
extern const _string_view _str_vu;

#if USE_ETL 
    extern etl::string<6*MAX_STRLN> _str_jumbo;
#else
    extern _string _str_jumbo;
#endif

extern _string _str_tiny; //{c_str_tiny};
extern _string _str_medium; //{c_str_medium};
extern _string _str_large; //{c_str_large};
#if USE_ETL
    extern etl::string<6*MAX_STRLN> _str_jumbo; 
    extern etl::string<6*MAX_STRLN> _str_jumbo2; 
#else 
    extern _string _str_jumbo; 
    extern _string _str_jumbo2;
#endif

void string_benchmark(uint32_t);

