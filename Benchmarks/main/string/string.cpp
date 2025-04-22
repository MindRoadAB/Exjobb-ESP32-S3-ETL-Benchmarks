#include "../common/common.hpp"
#include "string.hpp"

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
    etl::string<6*MAX_STRLN> _str_jumbo2; 
#else 
    _string _str_jumbo{c_str_jumbo};
    _string _str_jumbo2; 
#endif

void
string_benchmark(uint32_t cycles) 
{
    CYCLE_GET_COUNT(
        _string s{c_str_tiny},
        cycles,
        "string: c_str_tiny"
    );

    CYCLE_GET_COUNT(
        _string s{c_str_medium},
        cycles,
        "string: c_str_medium"
    );

    CYCLE_GET_COUNT(
        _string s{c_str_large},
        cycles,
        "string: c_str_large"
    );

    CYCLE_GET_COUNT(
        _string s{c_str_jumbo},
        cycles,
        "string: c_str_jumbo"
    );

     CYCLE_GET_COUNT_RETURN(
        _str_tiny[1],
        cycles,
        "string: _str_tiny[1]"
    );

    CYCLE_GET_COUNT_RETURN(
        _str_medium[11],
        cycles,
        "string: _str_medium[11]"
    );

    CYCLE_GET_COUNT_RETURN(
        _str_large[88],
        cycles,
        "string: _str_large[88]"
    );

    CYCLE_GET_COUNT_RETURN(
        _str_jumbo[888],
        cycles,
        "string: _str_jumbo[888]"
    );

    CYCLE_GET_COUNT(
        _reverse(_str_tiny.begin(), _str_tiny.end()),
        cycles,
        "string: reverse _str_tiny"
    );

    CYCLE_GET_COUNT(
        _reverse(_str_medium.begin(), _str_medium.end()),
        cycles,
        "string: reverse _str_medium"
    );

    CYCLE_GET_COUNT(
        _reverse(_str_large.begin(), _str_large.end()),
        cycles,
        "string: reverse _str_large"
    );

    CYCLE_GET_COUNT(
        _reverse(_str_jumbo.begin(), _str_jumbo.end()),
        cycles,
        "string: reverse _str_jumbo"
    );

    // Undo previous reverse operations
    _reverse(_str_tiny.begin(), _str_tiny.end());
    _reverse(_str_medium.begin(), _str_medium.end());
    _reverse(_str_large.begin(), _str_large.end());
    _reverse(_str_jumbo.begin(), _str_jumbo.end());
    
    CYCLE_GET_COUNT(
        _str_jumbo.clear(),
        cycles,
        "_str_jumbo.clear()"
    );

    CYCLE_GET_COUNT_MUTATE(
        {},                         
        _str_jumbo += "a",         
        _str_jumbo.pop_back(),     
        cycles,
        "_str_jumbo += 'a'"
    );
    
    CYCLE_GET_COUNT_MUTATE(
        {}, 
        _str_jumbo.append(c_str_jumbo),
        _str_jumbo.erase(_str_jumbo.size() - strlen(c_str_jumbo)),
        cycles,
        "string: append c_str_jumbo"
    );

    CYCLE_GET_COUNT_RETURN(
        (_str_jumbo == _str_jumbo2),
        cycles,
        "compare: _str_jumbo == _str_jumbo2"
    );

}


