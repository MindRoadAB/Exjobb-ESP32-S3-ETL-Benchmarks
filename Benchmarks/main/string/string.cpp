#include "string.hpp"

constexpr const char *tag = "string";

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

const _string_view _str_vu{c_str_large};

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
        _string(),
        cycles,
        "string: string s()"
    );

    CYCLE_GET_COUNT(
        _string s(c_str_large),
        cycles,
        "string: string s(c_str_large)"
    );

    size_t c_str_large_len = strnlen(c_str_large, MAX_STRLN); 

    CYCLE_GET_COUNT(
        _string s(c_str_large, c_str_large_len),
        cycles,
        "string: string s(c_str_large, size)"
    );

    CYCLE_GET_COUNT(
        _string s(MAX_STRLN, 'a'),
        cycles,
        "string: string s(MAX_STRLN, 'a')"
    );

    CYCLE_GET_COUNT(
        _string s(_str_vu),
        cycles,
        "string: string s(string_view v)"
    );

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

    CYCLE_GET_COUNT_RETURN(
        _str_large.at(88),
        cycles,
        "string: _str_large.at(88)"
    );

    CYCLE_GET_COUNT_RETURN(
        _str_large.front(),
        cycles,
        "string: _str_large.front()"
    );

    CYCLE_GET_COUNT_RETURN(
        _str_large.back(),
        cycles,
        "string: _str_large.back()"
    );

    CYCLE_GET_COUNT_RETURN(
        _str_large.begin(),
        cycles,
        "string: _str_large.begin()"
    ); 

    CYCLE_GET_COUNT_RETURN(
        _str_large.end(),
        cycles,
        "string: _str_large.end()"
    );

    CYCLE_GET_COUNT_RETURN(
        _str_large.rbegin(),
        cycles,
        "string: _str_large.rbegin()"
    );

    CYCLE_GET_COUNT_RETURN(
        _str_large.rend(),
        cycles,
        "string: _str_large.rend()"
    );
    
    CYCLE_GET_COUNT_RETURN(
        _str_large.data(),
        cycles,
        "string: _str_large.data()"
    );

    CYCLE_GET_COUNT_RETURN(
        _str_large.empty(),
        cycles,
        "string: _str_large.empty()"
    );

    CYCLE_GET_COUNT_RETURN(
        _str_large.max_size(),
        cycles,
        "string: _str_large.max_size()"
    );

    CYCLE_GET_COUNT_RETURN(
        _str_large.capacity(),
        cycles,
        "string: _str_large.capacity()"
    );

    CYCLE_GET_COUNT_RETURN(
        _str_large.size(),
        cycles,
        "string: _str_large.size()"
    );

    CYCLE_GET_COUNT(
        _str_tiny.resize(MAX_STRLN),
        cycles,
        "string: _str_tiny.resize(MAX_STRLN)"
    );

    _str_tiny.resize(2); /** undo resize above */

    CYCLE_GET_COUNT(
         _str_tiny.resize_and_overwrite(10, [](char* data, size_t capacity) -> size_t {
            const char* new_text = "world";
            std::memcpy(data, new_text, 5);
            return 5; }),
        cycles,
        "string: _str_tiny.resize_and_overwrite(size, operation)"

    );

    _str_tiny = "hi"; /** undo resize and overwrite */

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

    /** Undo previous reverse operations */
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

    CYCLE_GET_COUNT_MUTATE(
        {},
        _str_medium.push_back('a'),
        _str_medium.pop_back(),
        cycles,
        "string: _str_medium.push_back('a')"
    );

    CYCLE_GET_COUNT_MUTATE(
        {},
        _str_medium.insert(3, "Hello"),
        _str_medium.erase(3, 5),
        cycles,
        "string: _str_medium.insert(3, 'Hello')"
    );


    CYCLE_GET_COUNT_MUTATE (
        {},
        _str_medium.insert(3, 5, 'a'),
        _str_medium.erase(3, 5),
        cycles,
        "string: _str_medium.insert(3, 5, 'a')"
    );

    CYCLE_GET_COUNT_MUTATE (
        {},
        _str_medium.insert(_str_medium.begin() + 3, 'a'),
        _str_medium.erase(_str_medium.begin() + 3),
        cycles,
        "string: _str_medium.insert(_str_medium.begin() + 3, 'a')"
    );

    CYCLE_GET_COUNT_MUTATE(
        {}, 
        _str_medium.erase(3, 5),
        _str_medium.insert(3, "lo Wo"),
        cycles,
        "string: _str_medium.erase(3, 5)"
    );
    
    CYCLE_GET_COUNT_MUTATE (
        {}, 
        _str_medium.erase(_str_medium.begin() + 3),
        _str_medium.insert(_str_medium.begin() + 3, 'l'),
        cycles,
        "string: _str_medium.erase(_str_medium.begin() + 3)"
    );

    const char *temp = "hello again"; 
    size_t temp_len = strlen(temp);
    
    size_t half_jumbo = _str_jumbo.size() / 2;
    auto substr = _str_jumbo.substr(half_jumbo, temp_len);

    CYCLE_GET_COUNT_MUTATE (
        _str_jumbo = c_str_jumbo;,
        _str_jumbo.replace(half_jumbo, temp_len, temp),
        _str_jumbo.replace(half_jumbo, temp_len, substr),
        cycles,
        "string: _str_jumbo.replace(middle_pos, \"hello again\")"
    );
    
    ESP_LOGI(tag, "DONE\n");    

}


