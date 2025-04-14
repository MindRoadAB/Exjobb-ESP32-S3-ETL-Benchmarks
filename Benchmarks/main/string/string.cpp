#include "../common/common.hpp"
#include "string.hpp"

int32_t cycle_count{};
int64_t time_count{};

const char *str_tiny = "hi";
const char *str_medium = "Hello World, it's a great day!";
const char *str_large = "Hello World, it's a great day!"
                        "Hello World, it's a great day!"
                        "Hello World, it's a great day!";

const char *str_jumbo = "Hello World, it's a great day!"
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
_string _str_tiny{str_tiny};
_string _str_medium{str_medium};
_string _str_large{str_large};
#if USE_ETL
    etl::string<6*MAX_STRLN> _str_jumbo{str_jumbo};
#else 
    _string _str_jumbo{str_jumbo};
#endif


void string_constructor(void)
{
    CYCLE_GET_COUNT (
        _string s{""}, 
        cycle_count,
        "constructor empty string"
    );
    
    CYCLE_GET_COUNT(
        _string s{str_tiny},
        cycle_count,
        "constructor str_tiny"
    );

    CYCLE_GET_COUNT(
        _string s{str_medium},
        cycle_count,
        "constructor str_medium"
    );

    CYCLE_GET_COUNT(
        _string s{str_large},
        cycle_count,
        "constructor str_large"
    );

    #if USE_ETL
        CYCLE_GET_COUNT(
            etl::string<6*MAX_STRLN> s{str_jumbo},
            cycle_count,
            "constructor str_jumbo"
        );
    #else
        CYCLE_GET_COUNT (
            _string s{str_jumbo}, 
            cycle_count,
            "constructor str_jumbo"
        );
    #endif

}

void string_elem_access(void)
{
    CYCLE_GET_COUNT_VAL (
        _str_tiny[1],
        cycle_count,
        "elem access operator[] _str_tiny"
    );

    CYCLE_GET_COUNT_VAL (
        _str_medium[11],
        cycle_count,
        "elem access operator[] _str_medium"
    );

    CYCLE_GET_COUNT_VAL (
        _str_large[88],
        cycle_count,
        "elem access operator[] _str_large"
    );

    CYCLE_GET_COUNT_VAL (
        _str_jumbo[888],
        cycle_count,
        "elem access operator[] _str_jumbo"
    );
    
    CYCLE_GET_COUNT_VAL (
        _str_tiny.at(1),
        cycle_count,
        "elem access .at() _str_tiny"
    );

    CYCLE_GET_COUNT_VAL (
        _str_medium.at(11),
        cycle_count,
        "elem access .at() _str_medium"
    );

    CYCLE_GET_COUNT_VAL (
        _str_large.at(88),
        cycle_count,
        "elem access .at() _str_large"
    );

    CYCLE_GET_COUNT_VAL (
        _str_jumbo.at(888),
        cycle_count,
        "elem access .at() _str_jumbo"
    );

}

void string_reverse(void)
{
    CYCLE_GET_COUNT (
        _reverse(_str_tiny.begin(), _str_tiny.end()),
        cycle_count, 
        "reverse string _str_tiny"
    );

    
    CYCLE_GET_COUNT (
        _reverse(_str_medium.begin(), _str_medium.end()),
        cycle_count, 
        "reverse string _str_medium"
    );
    CYCLE_GET_COUNT (
        _reverse(_str_large.begin(), _str_large.end()),
        cycle_count, 
        "reverse string _str_large"
    );

    CYCLE_GET_COUNT (
        _reverse(_str_jumbo.begin(), _str_jumbo.end()),
        cycle_count,
        "reverse string _str_jumbo"
    );

    _reverse(_str_tiny.begin(), _str_tiny.end());
    _reverse(_str_medium.begin(), _str_medium.end());
    _reverse(_str_large.begin(), _str_large.end());
    _reverse(_str_jumbo.begin(), _str_jumbo.end());
}


void string_reverse_copy(void)
{
    CYCLE_GET_COUNT (
        std::reverse_copy(std::begin(_str_tiny), std::end(_str_tiny), std::begin(_str_tiny)), 
        cycle_count, 
        "reverse_copy string _str_tiny"
    );

    printf("\n_str_tiny: %s\n\n", _str_tiny.c_str());

    CYCLE_GET_COUNT (
        std::reverse_copy(std::begin(_str_medium), std::end(_str_medium), std::begin(_str_medium)), 
        cycle_count, 
        "reverse_copy string _str_medium"
    );
    
    CYCLE_GET_COUNT (
        std::reverse_copy(std::begin(_str_large), std::end(_str_large), std::begin(_str_large)), 
        cycle_count, 
        "reverse_copy string _str_large"
    );
    
    CYCLE_GET_COUNT (
        std::reverse_copy(std::begin(_str_jumbo), std::end(_str_jumbo), std::begin(_str_jumbo)), 
        cycle_count, 
        "reverse_copy string _str_jumbo"
    );
}

void string_append(void)
{
    _str_jumbo.clear();
    CYCLE_GET_COUNT_CUSTOM_ITERS (
        _str_jumbo += "a"  ,
        cycle_count,
        1000, 
        "string append str_jumbo"
    );
    printf("str_jumbo is: %s and size is: %u\n", _str_jumbo.c_str(), _str_jumbo.length()); 
    printf("str_tiny is: %s and size is: %u\n", _str_tiny.c_str(), _str_tiny.length()); 
}

void task_string(void *arg)
{
    string_constructor();
    string_elem_access();
    string_reverse(); 
    //string_reverse_copy(); 
    string_append();
    
    
    vTaskDelete(nullptr);
}


