#pragma once
#include <string>

#define MAX_DEQUE_SIZE 64

#if USE_ETL
    #include <etl/string.h>
    #include <etl/deque.h>
    #define STRLN 4
    using _deque_int = etl::deque<int, MAX_DEQUE_SIZE>;
    using _deque_string = etl::deque<etl::string<STRLN>, MAX_DEQUE_SIZE>;
#else
    #include <deque>
    
    using _deque_int = std::deque<int>;
    using _deque_string = std::deque<std::string>; 
#endif

extern _deque_int deq_i1;
extern _deque_int deq_i2;
extern _deque_int deq_iempty;
extern _deque_string deq_s1;
extern _deque_string deq_s2;
extern _deque_string deq_sempty;


