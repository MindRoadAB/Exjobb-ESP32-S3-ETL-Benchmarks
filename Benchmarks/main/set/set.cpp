#include "set.hpp"
#include "../common/common.hpp"

_set_int uset_i1 = 
{
     0,  1,  2,  3,  4,  5,  6,  7,  
     8,  9, 10, 11, 12, 13, 14, 15, 
    16, 17, 18, 19, 20, 21, 22, 23, 
    24, 25, 26, 27, 28, 29, 30, 31
};

_set_int uset_i2 = 
{
    234, 9389, 312, 412, 123, 777
};

_set_int uset_iempty = {};

_set_string uset_s1 = 
{
    "joe", "syd", "sid", "jef",
    "bob", "bil", "jok", "rob",
    "kim", "jim", "my", "war" 
};

_set_string uset_s2 = 
{
    "awk", "sed", "bsh", "bsd"
};

_set_string uset_sempty = {};

void
set_benchmark(uint32_t cycles)
{
    CYCLE_GET_COUNT (
        _set_int s{},
        cycles,
        "unordered_set<int>: _set_int s{}"
    );

    CYCLE_GET_COUNT (
        _set_int s{uset_i1},
        cycles,
        "unordered_set<int>: _set_int s{copy}"
    );
    
    CYCLE_GET_COUNT (
        _set_int s(uset_i1.begin(), uset_i1.end()),
        cycles,
        "unordered_set<int>: _set_int s(begin, end)" 
    );
    
    CYCLE_GET_COUNT (
        _set_int s = uset_i1,
        cycles,
        "unordered_set<int>: operator= constructor" 
    );

    CYCLE_GET_COUNT (
        _set_string s{},
        cycles,
        "unordered_set<string>: _set_string s{}"
    );

    CYCLE_GET_COUNT (
        _set_string s{uset_s1},
        cycles,
        "unordered_set<string>: _set_string s{copy}"
    );

    CYCLE_GET_COUNT (
        _set_string s(uset_s1.begin(), uset_s1.end()),
        cycles,
        "unordered_set<string>: _set_str s(begin, end)" 
    );
    
    CYCLE_GET_COUNT (
        _set_string s = uset_s1,
        cycles,
        "unordered_set<string>: operator= constructor" 
    );

    CYCLE_GET_COUNT (
        uset_i1.find(20),
        cycles,
        "unordered_set<int>: uset.find(key)" 
    );

    CYCLE_GET_COUNT (
        uset_s1.find("jok"),
        cycles,
        "unordered_set<string>: uset.find(key)" 
    );

    CYCLE_GET_COUNT (
        uset_s1.contains("bsd"),
        cycles,
        "unordered_set<string>: uset.contains(key)" 
    );

    CYCLE_GET_COUNT (
        uset_i1.contains(777),
        cycles,
        "unordered_set<int>: uset.contains(key)" 
    );
    
    CYCLE_GET_COUNT_MUTATE (
        {},
        uset_i1.insert(314),
        uset_i1.erase(314),
        cycles, 
        "unordered_set<int>: uset.insert(key)" 
    );

    CYCLE_GET_COUNT_MUTATE (
        {},
        uset_i1.insert(uset_i2.begin(), uset_i2.end()),
        { for (const auto& k: uset_i2) uset_i1.erase(k); },
        cycles, 
        "unordered_set<int>: uset.insert(begin, end)" 
    );

    CYCLE_GET_COUNT_MUTATE (
        {},
        uset_i1.insert(uset_i1.begin(), 999),
        uset_i1.erase(999),
        cycles, 
        "unordered_set<int>: uset.insert(hint, key)" 
    );

    CYCLE_GET_COUNT_MUTATE (
        {},
        uset_s1.insert("zoo"),
        uset_s1.erase("zoo"),
        cycles, 
        "unordered_set<string>: uset.insert(key)" 
    );

    CYCLE_GET_COUNT_MUTATE (
        {},
        uset_s1.insert(uset_s2.begin(), uset_s2.end()),
        { for (const auto& k: uset_s2) uset_s1.erase(k); },
        cycles, 
        "unordered_set<string>: uset.insert(begin, end)" 
    );

    CYCLE_GET_COUNT_MUTATE (
        {},
        uset_s1.insert(uset_s1.begin(), "tim"),
        uset_s1.erase("tim"),
        cycles, 
        "unordered_set<int>: uset.insert(hint, key)" 
    );

    CYCLE_GET_COUNT_MUTATE (
        {},
        uset_i1.erase(29),
        uset_i1.insert(29),
        cycles, 
        "unordered_set<int>: uset.erase(key)"
    );

    CYCLE_GET_COUNT_MUTATE (
        {},
        uset_s1.erase("my"),
        uset_s1.insert("my"),
        cycles, 
        "unordered_set<string>: uset.erase(key)"
    );
}