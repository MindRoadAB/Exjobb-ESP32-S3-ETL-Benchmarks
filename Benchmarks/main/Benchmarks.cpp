#include "common/common.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string/string.hpp"
#include "vector/vector.hpp"
#include "map/map.hpp"


int32_t cycles{0};


extern "C" void app_main(void)
{
    #if USE_ETL
        printf("Using ETL...\n");
    #else
        printf("Using libstdc++...\n"); 
    #endif

    /** For raw benchmarks, avoid using FreeRTOS tasks */
    /** TODO: Consider an expanded FOR_EACH macro */
    
    // /** Constructor with C-strings */
    // TEST_CONSTRUCTOR(
    //     _string s{c_str_tiny},
    //     cycles,
    //     "string: c_str_tiny"
    // );
    
    // TEST_CONSTRUCTOR(
    //     _string s{c_str_medium},
    //     cycles,
    //     "string: c_str_medium"
    // );

    // TEST_CONSTRUCTOR(
    //     _string s{c_str_large},
    //     cycles,
    //     "string: c_str_large"
    // );

    
    // TEST_CONSTRUCTOR(
    //     #if USE_ETL
    //         etl::string<6*MAX_STRLN> s{c_str_jumbo},
    //     #else 
    //         _string s{c_str_jumbo},
    //     #endif
    //     cycles,
    //     "string: c_str_jumbo"
    // );

    // /** Operator[] */
    // TEST_ELEM_ACCESS(
    //     _str_tiny[1],
    //     cycles,
    //     "string: _str_tiny"
    // );

    // TEST_ELEM_ACCESS(
    //     _str_medium[11],
    //     cycles,
    //     "string: _str_medium"
    // );
    
    // TEST_ELEM_ACCESS(
    //     _str_large[88],
    //     cycles,
    //     "string: _str_large"
    // );
    
    // TEST_ELEM_ACCESS(
    //     _str_jumbo[888],
    //     cycles,
    //     "string: _str_jumbo"
    // );

    // /** [std|etl]::reverse(begin, end) */
    // TEST_REVERSE(
    //     _str_tiny,
    //     cycles,
    //     "string: _str_tiny"
    // );

    // TEST_REVERSE(
    //     _str_medium,
    //     cycles,
    //     "string: _str_medium"
    // );
    
    // TEST_REVERSE(
    //     _str_large,
    //     cycles,
    //     "string: _str_large"
    // );
    
    // TEST_REVERSE(
    //     _str_jumbo,
    //     cycles,
    //     "string: _str_jumbo"
    // );

    // /** Undo previous operations */

    // _reverse(_str_tiny.begin(), _str_tiny.end());
    // _reverse(_str_medium.begin(), _str_medium.end());
    // _reverse(_str_large.begin(), _str_large.end());
    // _reverse(_str_jumbo.begin(), _str_jumbo.end());

    // /** _str_jumbo is already so large, clear it, do 1000 iterations (works well for both etl/libstdc++)
    //  *  clock it.
    //  */
    // CYCLE_GET_COUNT(
    //     _str_jumbo.clear(),
    //     cycles,
    //     "_str_jumbo.clear()"
    // );
    
    // TEST_APPEND(
    //     _str_jumbo += "a",
    //     cycles,
    //     1000,
    //     "string: _str_jumbo"
    // )
    //  /** Clear it back */
    // _str_jumbo.clear();
    
    // /** append c_str_jumbo */
    // _str_jumbo.append(c_str_jumbo);

    
    // TEST_COMPARE(
    //     _str_jumbo, 
    //     _str_jumbo2,
    //     cycles, 
    //     "_str_jumbo, c_str_jumbo, "
    // );


    TEST_CONSTRUCTOR (
        _vector_int i{}, 
        cycles, 
        "vector: _vector_int i{}"
    );

    TEST_CONSTRUCTOR (
        _vector_int i{ints}, 
        cycles, 
        "vector: _vector_int i{ints} (128 members) "
    );

    TEST_CONSTRUCTOR (
        _vector_int i(ints.begin(), ints.end()),
        cycles, 
        "_vector_int i(ints.begin(), ints.end())"
    );

    printf("size, cap: %u, %u\n", i_blank.size(), i_blank.capacity());
    TEST_APPEND (
        i_blank.push_back(111),
        cycles, 
        i_blank.size() - 1,
        "_vector_int push_back(111)"
    );

    if (i_blank.empty())
        printf("empty\n");
    else
        printf("not empty\n");

    printf("size, cap: %u, %u\n", i_blank.size(), i_blank.capacity());
    TEST_POP_BACK (
        i_blank, 
        cycles,
        ints.size(), 
        "_vector_int ints.pop_back()"
    );

    /** Reconstruct ints */
    i_blank = ints; 
    
    
    
    printf("size, cap: %u, %u\n", i_blank.size(), i_blank.capacity());
    
    // for (size_t i{0}; i < 128; i++)
    //     printf("ints: %d\n", ints[i]); 
    
    // printf("\n");
    
    // std::vector<int> vec = {1, 2, 3, 4, 5};
    
    // for (size_t i = 0; i < vec.size(); i++) 
    //     printf("%d\n", vec[i]); 
    
    // printf("vec.size, capacity: %u, %u\n", vec.size(), vec.capacity());
    // vec.pop_back();
    
    // printf("vec.size, capacity: %u, %u\n", vec.size(), vec.capacity());
    
    // for (size_t i = 0; i < vec.size(); i++) 
    //     printf("%d\n", vec[i]); 
    
    
    // ints.pop_back();
    
    // printf("\n");
    
    // for (size_t i{0}; i < 128; i++)
    //     printf("ints: %d\n", ints[i]); 
    
    
    /** Get the middle value, remove it to make room for insertion operations */
    size_t middle = i_blank.size() / 2;
    auto value = i_blank[middle]; 
    i_blank.erase(i_blank.begin() + middle);
    /************************************************************************ */ 
    
    TEST_VECTOR_INSERT (
        i_blank,
        middle, 
        2222,  
        cycles, 
        100, 
        "_vector_int insert"
    )
    
    /** Put the removed value back before next test. */
    i_blank.insert(i_blank.begin() + middle, value);
    /************************************************************************ */ 
    
    TEST_VECTOR_ERASE (
        i_blank, 
        middle,
        cycles, 
        100,
        "_vector_int erase()"
    );

    printf("\n\n");

    TEST_CONSTRUCTOR (
        //#if USE_ETL
            _map m{}, 
        // #else 
        //     _u_map_int_string x{},
        // #endif 
        cycles,
        "_u_map_int_string empty constructor"
    );
    
    TEST_CONSTRUCTOR (
        //#if USE_ETL
            _map m{umap_1}, 
        // #else 
        //     _u_map_int_string x{},
        // #endif 
        cycles,
        "_u_map_int_string copy constructor"
    );

    TEST_CONSTRUCTOR (
       // #if USE_ETL
            _map m(umap_1.begin(), umap_1.end()), 
        //#else 
         //   _u_map_int_string x{},
        //#endif 
        cycles,
        "_u_map_int_string iterator constructor"
    );

    TEST_ELEM_ACCESS (
        umap_1[2], 
        cycles,
        "u_map_int_string operator[]"
    );

    for (auto& k: umap_1)
        printf("<%d, %s>\n", k.first, k.second.c_str());
    umap_1.insert({11, "box"});
    for (auto& k: umap_1)
        printf("\n\n<%d, %s>\n", k.first, k.second.c_str());

    umap_1.erase(11);
    printf("BREAK\n");
    for (auto& k: umap_1)
        printf("\n<%d, %s>\n", k.first, k.second.c_str());

    TEST_MAP_INSERT (
        umap_1,
        std::make_pair(11, "box"),
        cycles,
        100,
        "u_map_int_string insert std::make_pair"
    );

    std::pair<int, std::string> mapElem = {11, "box"};

    TEST_MAP_INSERT (
        umap_1,
        mapElem,
        cycles,
        100,
        "u_map_int_string insert object"
    );
    
    auto first = umap_1.find(1);
    printf("first elem: %d, %s\n", first->first, first->second.c_str()); 
    
    mapElem = std::make_pair(first->first, first->second); 
    
    TEST_MAP_ERASE (
        umap_1,
        mapElem,
        cycles,
        100,
        "u_map_int_string_erase object"
    );

    for (auto& k: umap_1)
        printf("\n<%d, %s>\n", k.first, k.second.c_str());

    TEST_FIND (
        umap_large,
        13,
        cycles,
        "umap_int_string find"
    );




















}


