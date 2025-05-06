#pragma once

#include "../common/common.hpp"

#include <algorithm>

#if USE_ETL
    #include <etl/vector.h>
    #define MAX_VECTLN 128 
    using _vector_int = etl::vector<int, MAX_VECTLN>; 
#else
    #include <vector>
    using _vector_int = std::vector<int>;
#endif

extern _vector_int i_blank;
extern _vector_int ints;  

void vector_benchmark(uint32_t);

