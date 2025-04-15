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


typedef struct {   // 24 bytes
    #if USE_ETL
        etl::string<24> a;
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

_vector_dummy d_blank{};
_vector_int i_blank{};

extern _vector_dummy dummy = 
{
    {"sdlkfj", 0},   {"sdlkjsdflkj", 1},   {"sdlfkjsdlkfj", 2},   {"sdlkfjsdflkjsdf", 3},
    {"sdlkfj", 4},   {"sdlkjsdflkj", 5},   {"sdlfkjsdlkfj", 6},   {"sdlkfjsdflkjsdf", 7},
    {"sdlkfj", 8},   {"sdlkjsdflkj", 9},   {"sdlfkjsdlkfj", 10},  {"sdlkfjsdflkjsdf", 11},
    {"sdlkfj", 12},  {"sdlkjsdflkj", 13},  {"sdlfkjsdlkfj", 14},  {"sdlkfjsdflkjsdf", 15},
    {"sdlkfj", 16},  {"sdlkjsdflkj", 17},  {"sdlfkjsdlkfj", 18},  {"sdlkfjsdflkjsdf", 19},
    {"sdlkfj", 20},  {"sdlkjsdflkj", 21},  {"sdlfkjsdlkfj", 22},  {"sdlkfjsdflkjsdf", 23},
    {"sdlkfj", 24},  {"sdlkjsdflkj", 25},  {"sdlfkjsdlkfj", 26},  {"sdlkfjsdflkjsdf", 27},
    {"sdlkfj", 28},  {"sdlkjsdflkj", 29},  {"sdlfkjsdlkfj", 30},  {"sdlkfjsdflkjsdf", 31},
    {"sdlkfj", 32},  {"sdlkjsdflkj", 33},  {"sdlfkjsdlkfj", 34},  {"sdlkfjsdflkjsdf", 35},
    {"sdlkfj", 36},  {"sdlkjsdflkj", 37},  {"sdlfkjsdlkfj", 38},  {"sdlkfjsdflkjsdf", 39},
    {"sdlkfj", 40},  {"sdlkjsdflkj", 41},  {"sdlfkjsdlkfj", 42},  {"sdlkfjsdflkjsdf", 43},
    {"sdlkfj", 44},  {"sdlkjsdflkj", 45},  {"sdlfkjsdlkfj", 46},  {"sdlkfjsdflkjsdf", 47},
    {"sdlkfj", 48},  {"sdlkjsdflkj", 49},  {"sdlfkjsdlkfj", 50},  {"sdlkfjsdflkjsdf", 51},
    {"sdlkfj", 52},  {"sdlkjsdflkj", 53},  {"sdlfkjsdlkfj", 54},  {"sdlkfjsdflkjsdf", 55},
    {"sdlkfj", 56},  {"sdlkjsdflkj", 57},  {"sdlfkjsdlkfj", 58},  {"sdlkfjsdflkjsdf", 59},
    {"sdlkfj", 60},  {"sdlkjsdflkj", 61},  {"sdlfkjsdlkfj", 62},  {"sdlkfjsdflkjsdf", 63},
    {"sdlkfj", 64},  {"sdlkjsdflkj", 65},  {"sdlfkjsdlkfj", 66},  {"sdlkfjsdflkjsdf", 67},
    {"sdlkfj", 68},  {"sdlkjsdflkj", 69},  {"sdlfkjsdlkfj", 70},  {"sdlkfjsdflkjsdf", 71},
};

_vector_int ints = 
{
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









