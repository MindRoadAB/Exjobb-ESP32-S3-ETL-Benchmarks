#include "vector.hpp"


constexpr const char *tag = "vector";

_vector_int i_blank{};

_vector_int ints = {
    0,     1,   2,   3,   4,   5,   6,   7,   8,   9,
    10,   11,  12,  13,  14,  15,  16,  17,  18,  19, 
    20,   21,  22,  23,  24,  25,  26,  27,  28,  29, 
    30,   31,  32,  33,  34,  35,  36,  37,  38,  39,
    40,   41,  42,  43,  44,  45,  46,  47,  48,  49,
    50,   51,  52,  53,  54,  55,  56,  57,  58,  59,
    60,   61,  62,  63,  64,  65,  66,  67,  68,  69,
    70,   71,  72,  73,  74,  75,  76,  77,  78,  79,
    80,   81,  82,  83,  84,  85,  86,  87,  88,  89,
    90,   91,  92,  93,  94,  95,  96,  97,  98,  99,
    100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
    110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 
    120, 121, 122, 123, 124, 125, 126, 127
};

static _vector_int i_blank2 = ints; 

void
vector_benchmark(uint32_t cycles)
{
    CYCLE_GET_COUNT (
        _vector_int i{}, 
        cycles, 
        "vector: _vector_int i{}"
    );
    
    CYCLE_GET_COUNT(
        _vector_int i(256, 7),
        cycles,
        "vector: construct with size and value (256, 7)"
    );
    
    CYCLE_GET_COUNT (
        _vector_int i{ints}, 
        cycles, 
        "vector: _vector_int i{ints} (128 members) "
    );

    CYCLE_GET_COUNT (
        _vector_int i(ints.begin(), ints.end()),
        cycles, 
        "_vector_int i(ints.begin(), ints.end())"
    );

    CYCLE_GET_COUNT_MUTATE (
        {},
        i_blank.push_back(111),
        i_blank.pop_back(),
        cycles,
        "_vector_int push_back(111)"
    );

    CYCLE_GET_COUNT_MUTATE (
        auto k = i_blank.back(),
        i_blank.pop_back(),
        i_blank.push_back(k),
        cycles,
        "_vector_int pop_back()"
    );

    i_blank = ints;

    size_t middle = i_blank.size() / 2; 
    auto value = i_blank[middle]; 
    i_blank.erase(i_blank.begin() + middle);

    CYCLE_GET_COUNT_MUTATE (
        {},
        i_blank.insert(i_blank.begin() + middle, 2222),
        i_blank.erase(i_blank.begin() + middle),
        cycles,
        "_vector_int insert(middle, 2222)" 
    );
    i_blank.insert(i_blank.begin() + middle, value);
    
    
    value = i_blank.front();
    i_blank.erase(i_blank.begin()); 
    
    CYCLE_GET_COUNT_MUTATE (
        {},
        i_blank.insert(i_blank.begin(), 2222),
        i_blank.erase(i_blank.begin()),
        cycles,
        "_vector_int insert(0, 2222)"
    );
    i_blank.insert(i_blank.begin(), value);

    CYCLE_GET_COUNT_MUTATE (
        auto k = i_blank[middle],
        i_blank.erase(i_blank.begin() + middle),
        i_blank.insert(i_blank.begin() + middle, k),
        cycles,
        "_vector_int erase(middle)"
    );

    CYCLE_GET_COUNT_MUTATE (
        auto k = i_blank[0],
        i_blank.erase(i_blank.begin()),
        i_blank.insert(i_blank.begin(), k), 
        cycles,
        "_vector_int erase(0)"
    ); 
    
    CYCLE_GET_COUNT_RETURN(
        (i_blank == i_blank2),
        cycles,
        "vector: i_blank == i_blank2"
    );
    
    CYCLE_GET_COUNT_RETURN(
        i_blank.rbegin(),
        cycles,
        "vector: i_blank.rbegin()"
    );

    CYCLE_GET_COUNT_RETURN(
        i_blank.rend(),
        cycles,
        "vector: i_blank.rend()"
    );

    CYCLE_GET_COUNT_RETURN(
        i_blank.at(middle),
        cycles,
        "vector: i_blank.at(middle)"
    );

    CYCLE_GET_COUNT_RETURN(
        i_blank.front(),
        cycles,
        "vector: i_blank.front()"
    );

    CYCLE_GET_COUNT_RETURN(
        i_blank.back(),
        cycles,
        "vector: i_blank.back()"
    );

    CYCLE_GET_COUNT_RETURN(
        i_blank.size(),
        cycles,
        "vector: i_blank.size()"
    );

    CYCLE_GET_COUNT_RETURN(
        i_blank.capacity(),
        cycles,
        "vector: i_blank.capacity()"
    );

    CYCLE_GET_COUNT_RETURN(
        i_blank.max_size(),
        cycles,
        "vector: i_blank.max_size()"
    );

    // Reserve
    CYCLE_GET_COUNT(
        i_blank.reserve(512),
        cycles,
        "vector: i_blank.reserve(512)"
    );

    // Resize
    CYCLE_GET_COUNT(
        i_blank.resize(256),
        cycles,
        "vector: i_blank.resize(256)"
    );

    // Reverse
    CYCLE_GET_COUNT(
        _reverse(i_blank.begin(), i_blank.end()),
        cycles,
        "vector: reverse i_blank"
    );
    _reverse(i_blank.begin(), i_blank.end());

    CYCLE_GET_COUNT_MUTATE(
        {},
        i_blank.insert(i_blank.begin() + middle, 10, 123),
        i_blank.erase(i_blank.begin() + middle, i_blank.begin() + middle + 10),
        cycles,
        "vector: insert 10 elements at middle"
    );

    CYCLE_GET_COUNT_MUTATE(
        {},
        i_blank.insert(i_blank.end(), ints.begin(), ints.begin() + 10),
        i_blank.erase(i_blank.end() - 10, i_blank.end()),
        cycles,
        "vector: insert range at end (append)"
    );

    CYCLE_GET_COUNT(
        i_blank.clear(),
        cycles,
        "vector: i_blank.clear()"
    );

    CYCLE_GET_COUNT_MUTATE(
        {},
        {
            i_blank.resize(100);
            std::fill(i_blank.begin(), i_blank.end(), 42);
        },
        i_blank.clear(),
        cycles,
        "vector: resize and fill"
    );

    ESP_LOGI(tag, "DONE\n");
}

