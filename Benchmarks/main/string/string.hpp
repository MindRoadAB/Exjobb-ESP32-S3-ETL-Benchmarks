#pragma once

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

const char *c_str_tiny = "hi";
const char *c_str_medium = "Hello World, it's a great day!";
const char *c_str_large = "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!";

const char *c_str_jumbo = "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!";
_string _str_tiny{c_str_tiny};
_string _str_medium{c_str_medium};
_string _str_large{c_str_large};
#if USE_ETL
    etl::string<6*MAX_STRLN> _str_jumbo{c_str_jumbo};
    etl::string<6*MAX_STRLN> _str_jumbo2{c_str_jumbo};
#else 
    _string _str_jumbo{c_str_jumbo};
    _string _str_jumbo2{c_str_jumbo};
#endif


