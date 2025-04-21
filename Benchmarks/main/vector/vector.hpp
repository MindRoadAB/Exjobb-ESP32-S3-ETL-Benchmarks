#pragma once

#include <algorithm>


#if USE_ETL
    #include <etl/vector.h>
    #include <etl/string.h> 
    #define MAX_VECTLN 128 
    using _vector_int = etl::vector<int, MAX_VECTLN>; 
#else
    #include <vector>
    #include <string>
    using _vector_int = std::vector<int>;
#endif

typedef struct {
    float a;
    float b;
    float c;
}Point;


typedef struct {  
    #if USE_ETL
        etl::string<24> a{};
    #else
        std::string a;
    #endif
        int b;
}Dummy;

#if USE_ETL
    using _vector_dummy = etl::vector<Dummy, MAX_VECTLN>;
#else
    using _vector_dummy = std::vector<Dummy>;
#endif 

extern _vector_dummy d_blank;
extern _vector_int i_blank;
extern _vector_dummy dummy;  
extern _vector_int ints;  

void vector_benchmark(uint32_t);

