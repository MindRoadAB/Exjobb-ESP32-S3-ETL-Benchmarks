#include "deque.hpp"

constexpr const char *tag = "deque";

_deque_int deq_i1 = 
{
    32, 904, 233, -234,
    2340, 142, 5958, 0, 
    123,  9, 123234, 234, 41,
    9999, 21412, 234, 3333,
    321, 20480, 11, 1, 14, 91,
    2, 13, 451
};

_deque_int deq_i2 = 
{
    3, 14
};

_deque_int deq_iempty = {};

_deque_string deq_s1 = 
{
    "a", "aa", "aaa", 
    "b", "bb", "bbb",
    "ab", "aba", "abb", 
    "ba", "bab", "baa",
    "c", "ca", "cb",
    "cc", "ac", "bc",
    "ccc", "cab", "bac",
    "cba", "caa", "cbb",
    "bcc", "bca"
};

_deque_string deq_s2 = 
{
    "joe", "blo"
};

_deque_string deq_sempty = {};

void
deque_benchmark(uint32_t cycles)
{
    CYCLE_GET_COUNT (
        _deque_int d{},
        cycles,
        "deque<int>: deque_int{}"
    );

    CYCLE_GET_COUNT (
        _deque_int d{deq_iempty},
        cycles,
        "deque<int>: deque_int{empty copy}"
    );

    CYCLE_GET_COUNT (
        _deque_int d{deq_i1},
        cycles,
        "deque<int>: deque_int{copy}"
    );
 
    CYCLE_GET_COUNT (
        _deque_int d(deq_i1.begin(), deq_i1.end()),
        cycles,
        "deque<int>: deque_int(begin, end)"
    );

    CYCLE_GET_COUNT (
        _deque_string s{},
        cycles,
        "deque<string>: deque_string{}"
    );

    CYCLE_GET_COUNT (
        _deque_string s{deq_sempty},
        cycles,
        "deque<string>: deque_string{empty copy}"
    );

    CYCLE_GET_COUNT (
        _deque_string s{deq_s1},
        cycles,
        "deque<string>: deque_string{copy}"
    );
 
    CYCLE_GET_COUNT (
        _deque_string s(deq_s1.begin(), deq_s1.end()),
        cycles,
        "deque<string>: deque_string(begin, end)"
    );
    
    size_t deq_i1_size = deq_i1.size();
    size_t deq_s1_size = deq_s1.size();
    CYCLE_GET_COUNT_RETURN (
        deq_i1[deq_i1_size/2], 
        cycles,
        "deque<int>: operator[middle]"
    );
 
    CYCLE_GET_COUNT_RETURN (
        deq_i1.front(), 
        cycles,
        "deque<int>: front()"
    );

    CYCLE_GET_COUNT_RETURN (
        deq_i1.back(), 
        cycles,
        "deque<int>: back()"
    );

    CYCLE_GET_COUNT_RETURN (
        deq_s1[deq_s1_size/2], 
        cycles,
        "deque<string>: operator[middle]"
    );

    CYCLE_GET_COUNT_RETURN (
        deq_s1.front(), 
        cycles,
        "deque<string>: front()"
    );
 
    CYCLE_GET_COUNT_RETURN (
        deq_s1.back(), 
        cycles,
        "deque<string>: back()" 
    ); 

    CYCLE_GET_COUNT_MUTATE (
        {},
        deq_i1.push_front(0),
        deq_i1.pop_front(),
        cycles,
        "deque<int>: push_front(val)"
    );

    CYCLE_GET_COUNT_MUTATE (
        {},
        deq_i1.push_back(0),
        deq_i1.pop_back(),
        cycles,
        "deque<int>: push_back(val)"
    );
    
    CYCLE_GET_COUNT_MUTATE (
        {},
        deq_i1.insert((deq_i1.begin() + deq_i1_size / 2), -22),
        deq_i1.erase(deq_i1.begin() + deq_i1_size/2),
        cycles,
        "deque<int>: insert(middle, val)"
    );
    
    CYCLE_GET_COUNT_MUTATE (
        auto pos = (deq_i1.begin() + deq_i1_size / 2),
        deq_i1.insert(pos, deq_i2.begin(), deq_i2.end()),
        deq_i1.erase(pos, pos + std::distance(deq_i2.begin(), deq_i2.end())), 
        cycles,
        "deque<int>: insert(pos, begin, end)"
    );
    
    CYCLE_GET_COUNT_MUTATE (
        auto k = deq_i1[deq_i1_size / 4],
        deq_i1.erase(deq_i1.begin() + deq_i1_size /4),
        deq_i1.insert(deq_i1.begin() + deq_i1_size / 4, k),
        cycles,
        "deque<int>: erase(pos)"
    );
    
    CYCLE_GET_COUNT_MUTATE (
        {},
        deq_s1.push_front("tja"),
        deq_s1.pop_front(),
        cycles,
        "deque<string>: push_front(val)"
    );

    CYCLE_GET_COUNT_MUTATE (
        {},
        deq_s1.push_back("boo"),
        deq_s1.pop_back(),
        cycles,
        "deque<string>: push_back(val)"
    );
    
    CYCLE_GET_COUNT_MUTATE (
        {},
        deq_s1.insert((deq_s1.begin() + deq_s1_size / 2), "tja"),
        deq_s1.erase(deq_s1.begin() + deq_s1_size/2),
        cycles,
        "deque<string>: insert(middle, val)"
    );
    
    CYCLE_GET_COUNT_MUTATE (
        auto pos = (deq_s1.begin() + deq_s1_size / 2),
        deq_s1.insert(pos, deq_s2.begin(), deq_s2.end()),
        deq_s1.erase(pos, pos + std::distance(deq_s2.begin(), deq_s2.end())), 
        cycles,
        "deque<string>: insert(pos, begin, end)"
    );
    
    CYCLE_GET_COUNT_MUTATE (
        auto k = deq_s1[deq_s1_size / 4],
        deq_s1.erase(deq_s1.begin() + deq_s1_size /4),
        deq_s1.insert(deq_s1.begin() + deq_s1_size / 4, k),
        cycles,
        "deque<string>: erase(pos)"
    );

    ESP_LOGI(tag, "DONE\n");
}
