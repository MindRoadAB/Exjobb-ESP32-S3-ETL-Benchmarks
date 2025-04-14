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

void time(void)
{

    TIME_TASK (
        _string s{""}, 
        cycle_count,
        "constructor empty string"
    );
    
    TIME_TASK (
        _string s{str_tiny},
        cycle_count,
        "constructor str_tiny"
    );

    TIME_TASK (
        _string s{str_medium},
        cycle_count,
        "constructor str_medium"
    );

    TIME_TASK(
        _string s{str_large},
        cycle_count,
        "constructor str_large"
    );

    #if USE_ETL
        TIME_TASK (
            etl::string<6*MAX_STRLN> s{str_jumbo},
            cycle_count,
            "constructor str_jumbo"
        );
    #else
        TIME_TASK (
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



void task_string(void *arg)
{
    string_constructor();
    string_elem_access();
    time(); 
    vTaskDelete(nullptr);
}


