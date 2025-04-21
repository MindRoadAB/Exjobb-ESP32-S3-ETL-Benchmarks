#include "common/common.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#if USE_STRING
    #include "string/string.hpp"
#elif USE_VECTOR
    #include "vector/vector.hpp"
#endif

#include "vector/vector.hpp"
#include "map/map.hpp"
#include "set/set.hpp"
#include "deque/deque.hpp"

int32_t cycles{0};

extern "C" void app_main(void)
{
    #if USE_ETL
        printf("Using ETL...\n");
    #else
        printf("Using libstdc++...\n"); 
    #endif

    #if USE_STRING
        printf("Benchmarking string operations...\n"); 
        string_benchmark(cycles);
    #elif USE_VECTOR
        printf("Benchmarking vector operations...\n"); 
        vector_benchmark(cycles); 
    #else

    #endif
}

// int32_t cycles{0};


// extern "C" void app_main(void)
// {
//     #if USE_ETL
//         printf("Using ETL...\n");
//     #else
//         printf("Using libstdc++...\n"); 
//     #endif

//     /** For raw benchmarks, avoid using FreeRTOS tasks */
//     /** TODO: Consider an expanded FOR_EACH macro */
    
//     // /** Constructor with C-strings */
//     // TEST_CONSTRUCTOR(
//     //     _string s{c_str_tiny},
//     //     cycles,
//     //     "string: c_str_tiny"
//     // );
    
//     // TEST_CONSTRUCTOR(
//     //     _string s{c_str_medium},
//     //     cycles,
//     //     "string: c_str_medium"
//     // );

//     // TEST_CONSTRUCTOR(
//     //     _string s{c_str_large},
//     //     cycles,
//     //     "string: c_str_large"
//     // );

    
//     // TEST_CONSTRUCTOR(
//     //     #if USE_ETL
//     //         etl::string<6*MAX_STRLN> s{c_str_jumbo},
//     //     #else 
//     //         _string s{c_str_jumbo},
//     //     #endif
//     //     cycles,
//     //     "string: c_str_jumbo"
//     // );

//     // /** Operator[] */
//     // TEST_ELEM_ACCESS(
//     //     _str_tiny[1],
//     //     cycles,
//     //     "string: _str_tiny"
//     // );

//     // TEST_ELEM_ACCESS(
//     //     _str_medium[11],
//     //     cycles,
//     //     "string: _str_medium"
//     // );
    
//     // TEST_ELEM_ACCESS(
//     //     _str_large[88],
//     //     cycles,
//     //     "string: _str_large"
//     // );
    
//     // TEST_ELEM_ACCESS(
//     //     _str_jumbo[888],
//     //     cycles,
//     //     "string: _str_jumbo"
//     // );

//     // /** [std|etl]::reverse(begin, end) */
//     // TEST_REVERSE(
//     //     _str_tiny,
//     //     cycles,
//     //     "string: _str_tiny"
//     // );

//     // TEST_REVERSE(
//     //     _str_medium,
//     //     cycles,
//     //     "string: _str_medium"
//     // );
    
//     // TEST_REVERSE(
//     //     _str_large,
//     //     cycles,
//     //     "string: _str_large"
//     // );
    
//     // TEST_REVERSE(
//     //     _str_jumbo,
//     //     cycles,
//     //     "string: _str_jumbo"
//     // );

//     // /** Undo previous operations */

//     // _reverse(_str_tiny.begin(), _str_tiny.end());
//     // _reverse(_str_medium.begin(), _str_medium.end());
//     // _reverse(_str_large.begin(), _str_large.end());
//     // _reverse(_str_jumbo.begin(), _str_jumbo.end());

//     // /** _str_jumbo is already so large, clear it, do 1000 iterations (works well for both etl/libstdc++)
//     //  *  clock it.
//     //  */
//     // CYCLE_GET_COUNT(
//     //     _str_jumbo.clear(),
//     //     cycles,
//     //     "_str_jumbo.clear()"
//     // );
    
//     // TEST_APPEND(
//     //     _str_jumbo += "a",
//     //     cycles,
//     //     1000,
//     //     "string: _str_jumbo"
//     // )
//     //  /** Clear it back */
//     // _str_jumbo.clear();
    
//     // /** append c_str_jumbo */
//     // _str_jumbo.append(c_str_jumbo);

    
//     // TEST_COMPARE(
//     //     _str_jumbo, 
//     //     _str_jumbo2,
//     //     cycles, 
//     //     "_str_jumbo, c_str_jumbo, "
//     // );


//     // TEST_CONSTRUCTOR (
//     //     _vector_int i{}, 
//     //     cycles, 
//     //     "vector: _vector_int i{}"
//     // );

//     // TEST_CONSTRUCTOR (
//     //     _vector_int i{ints}, 
//     //     cycles, 
//     //     "vector: _vector_int i{ints} (128 members) "
//     // );

//     // TEST_CONSTRUCTOR (
//     //     _vector_int i(ints.begin(), ints.end()),
//     //     cycles, 
//     //     "_vector_int i(ints.begin(), ints.end())"
//     // );

//     // printf("size, cap: %u, %u\n", i_blank.size(), i_blank.capacity());
//     // TEST_APPEND (
//     //     i_blank.push_back(111),
//     //     cycles, 
//     //     i_blank.size() - 1,
//     //     "_vector_int push_back(111)"
//     // );

//     // if (i_blank.empty())
//     //     printf("empty\n");
//     // else
//     //     printf("not empty\n");

//     // printf("size, cap: %u, %u\n", i_blank.size(), i_blank.capacity());
//     // TEST_POP_BACK (
//     //     i_blank, 
//     //     cycles,
//     //     ints.size(), 
//     //     "_vector_int ints.pop_back()"
//     // );

//     // /** Reconstruct ints */
//     // i_blank = ints; 
    
    
    
//     // printf("size, cap: %u, %u\n", i_blank.size(), i_blank.capacity());
    
//     // // for (size_t i{0}; i < 128; i++)
//     // //     printf("ints: %d\n", ints[i]); 
    
//     // // printf("\n");
    
//     // // std::vector<int> vec = {1, 2, 3, 4, 5};
    
//     // // for (size_t i = 0; i < vec.size(); i++) 
//     // //     printf("%d\n", vec[i]); 
    
//     // // printf("vec.size, capacity: %u, %u\n", vec.size(), vec.capacity());
//     // // vec.pop_back();
    
//     // // printf("vec.size, capacity: %u, %u\n", vec.size(), vec.capacity());
    
//     // // for (size_t i = 0; i < vec.size(); i++) 
//     // //     printf("%d\n", vec[i]); 
    
    
//     // // ints.pop_back();
    
//     // // printf("\n");
    
//     // // for (size_t i{0}; i < 128; i++)
//     // //     printf("ints: %d\n", ints[i]); 
    
    
//     // /** Get the middle value, remove it to make room for insertion operations */
//     // size_t middle = i_blank.size() / 2;
//     // auto value = i_blank[middle]; 
//     // i_blank.erase(i_blank.begin() + middle);
//     // /************************************************************************ */ 
    
//     // TEST_VECTOR_INSERT (
//     //     i_blank,
//     //     middle, 
//     //     2222,  
//     //     cycles, 
//     //     100, 
//     //     "_vector_int insert"
//     // )
    
//     // /** Put the removed value back before next test. */
//     // i_blank.insert(i_blank.begin() + middle, value);
//     // /************************************************************************ */ 
    
//     // TEST_VECTOR_ERASE (
//     //     i_blank, 
//     //     middle,
//     //     cycles, 
//     //     100,
//     //     "_vector_int erase()"
//     // );

//     // printf("\n\n");

//     // TEST_CONSTRUCTOR (
//     //     _map m{}, 
//     //     cycles,
//     //     "_u_map_int_string empty constructor"
//     // );
    
//     // TEST_CONSTRUCTOR (
//     //     _map m{umap_1}, 
//     //     cycles,
//     //     "_u_map_int_string copy constructor"
//     // );

//     // TEST_CONSTRUCTOR (
//     //     _map m(umap_1.begin(), umap_1.end()), 
//     //     cycles,
//     //     "_u_map_int_string iterator constructor"
//     // );

//     // TEST_ELEM_ACCESS (
//     //     umap_1[2], 
//     //     cycles,
//     //     "u_map_int_string operator[]"
//     // );

//     // TEST_MAP_INSERT_HINT (
//     //     umap_1,
//     //     umap_1.begin(),
//     //     std::make_pair(11, "box"),
//     //     cycles,
//     //     100,
//     //     "u_map_int_string insert(iterator, std::make_pair)"
//     // );

//     // TEST_MAP_INSERT (
//     //     umap_1,
//     //     std::make_pair(11, "box"),
//     //     cycles,
//     //     100,
//     //     "u_map_int_string insert(std::make_pair)"
//     // );
        
//     // TEST_MAP_INSERT (
//     //     umap_1,
//     //     mapElem,
//     //     cycles,
//     //     100,
//     //     "u_map_int_string insert(pair)"
//     // );
    
//     // TEST_MAP_INSERT_RANGE (
//     //     umap_large, 
//     //     umap_2,
//     //     cycles,
//     //     100,
//     //     "u_map_int_string insert(range)"
//     // );

//     // auto first = umap_1.find(1);
//     // mapElem = std::make_pair(first->first, first->second); 
    
//     // TEST_MAP_ERASE (
//     //     umap_1,
//     //     mapElem,
//     //     cycles,
//     //     100,
//     //     "u_map_int_string_erase object"
//     // );

//     // TEST_FIND (
//     //     umap_large,
//     //     13,
//     //     cycles,
//     //     "umap_int_string find"
//     // );
// /********************************************************************* */

//     // TEST_CONSTRUCTOR (
//     //     _set_int s{},
//     //     cycles,
//     //     "u_set_int empty constructor"
//     // );
    
//     // TEST_CONSTRUCTOR (
//     //     _set_string s{},
//     //     cycles,
//     //     "u_set_string empty constructor"
//     // );
    
//     // TEST_CONSTRUCTOR (
//     //     _set_int s{uset_i1},
//     //     cycles,
//     //     "u_set_int copy constructor no bucket count"
//     // );

//     // TEST_CONSTRUCTOR (
//     //     _set_string s{uset_s1},
//     //     cycles,
//     //     "u_set_string copy constructor no bucket count"
//     // );
    
//     // TEST_CONSTRUCTOR (
//     //     _set_int s(uset_i1.begin(), uset_i1.end()),
//     //     cycles,
//     //     "u_set_int iterator constructor no bucket count"
//     // )

//     // TEST_CONSTRUCTOR (
//     //     _set_string s(uset_s1.begin(), uset_s1.end()),
//     //     cycles,
//     //     "u_set_string iterator constructor no bucket count"
//     // )
    
//     // #if !USE_ETL
//     //     constexpr size_t buckets = 64;
//     //     TEST_CONSTRUCTOR (
//     //         _set_int s(buckets),
//     //         cycles, 
//     //         "u_set_int empty with bucket count"
//     //     );
        
//     //     TEST_CONSTRUCTOR (
//     //         _set_int s(uset_i1.begin(), uset_i1.end(), uset_i1.bucket_count(), uset_i1.hash_function(), uset_i1.key_eq()),
//     //         cycles, 
//     //         "u_set_int iterator constructor with bucket count"
//     //     );

//     //     TEST_CONSTRUCTOR (
//     //         _set_string s(buckets),
//     //         cycles, 
//     //         "u_set_string empty with bucket count"
//     //     );
        
//     //     TEST_CONSTRUCTOR (
//     //         _set_string s(uset_s1.begin(), uset_s1.end(), uset_s1.bucket_count(), uset_s1.hash_function(), uset_s1.key_eq()),
//     //         cycles, 
//     //         "u_set_string iterator constructor with bucket count and hash and key"
//     //     );
//     // #endif

//     // TEST_CONSTRUCTOR (
//     //     _set_int s = uset_i1,
//     //     cycles, 
//     //     "u_set_int operator= constructor"
//     // );

//     // TEST_CONSTRUCTOR (
//     //     _set_string s = uset_s1,
//     //     cycles, 
//     //     "u_set_string operator= constructor"
//     // );
    
//     // TEST_ELEM_ACCESS (
//     //     uset_i1.find(20),
//     //     cycles, 
//     //     "uset_int uset_i1.find()"
//     // );

//     // TEST_ELEM_ACCESS (
//     //     uset_s1.find("jok"),
//     //     cycles, 
//     //     "uset_string uset_s1.find()"
//     // );
    
//     // TEST_ELEM_ACCESS (
//     //     uset_s2.find("bsd"),
//     //     cycles, 
//     //     "uset_string uset_s2.find()"
//     // );

//     // TEST_CONTAINS (
//     //     uset_s1, 
//     //     "bsd",
//     //     cycles, 
//     //     "uset_string sset2.contains()"
//     // );

//     // TEST_CONTAINS (
//     //     uset_i1, 
//     //     777,
//     //     cycles,
//     //     "uset_int uset_i1.contains()"
//     // );

//     // TEST_SET_INSERT (
//     //     uset_i1, 
//     //     314,
//     //     cycles,
//     //     100, 
//     //     "uset_int insert()"
//     // );

//     // TEST_SET_INSERT (
//     //     uset_s1, 
//     //     "zoo",
//     //     cycles,
//     //     100,
//     //     "uset_string insert()"
//     // );

//     // TEST_SET_INSERT_RANGE (
//     //     uset_i1, 
//     //     uset_i2,
//     //     cycles,
//     //     100,
//     //     "uset_int insert range"
//     // ); 

//     // TEST_SET_INSERT_RANGE (
//     //     uset_s1, 
//     //     uset_s2,
//     //     cycles,
//     //     100,
//     //     "uset_string insert range"
//     // ); 

//     // TEST_SET_INSERT_HINT (
//     //     uset_i1,
//     //     uset_i1.begin(),
//     //     999,
//     //     cycles,
//     //     100, 
//     //     "uset_int insert hint"
//     // );

//     // TEST_SET_INSERT_HINT (
//     //     uset_s1,
//     //     uset_s1.begin(),
//     //     "tim",
//     //     cycles,
//     //     100,
//     //     "uset_string insert hint"
//     // );

//     // TEST_SET_ERASE (
//     //     uset_i1,
//     //     29,
//     //     cycles,
//     //     100, 
//     //     "uset_int erase"
//     // );
    
//     // TEST_SET_ERASE (
//     //     uset_s1,
//     //     "my",
//     //     cycles,
//     //     100, 
//     //     "uset_string erase"
//     // );



// /********************************************************************** */

//     TEST_CONSTRUCTOR (
//         _deque_int d{},
//         cycles,
//         "deque_int empty constructor"
//     );

//     TEST_CONSTRUCTOR (
//         _deque_string d{},
//         cycles,
//         "deque_string empty constructor"
//     );

//     TEST_CONSTRUCTOR (
//         _deque_int d{deq_iempty},
//         cycles,
//         "deque_int empty constructor copy"
//     );

//     TEST_CONSTRUCTOR (
//         _deque_string d{deq_sempty},
//         cycles,
//         "deque_string empty constructor copy"
//     );

//     TEST_CONSTRUCTOR (
//         _deque_int d{deq_i1},
//         cycles,
//         "deque_int copy constructor"
//     );

//     TEST_CONSTRUCTOR (
//         _deque_string d{deq_s1},
//         cycles,
//         "deque_string copy constructor"
//     );

//     TEST_CONSTRUCTOR (
//         _deque_int d(deq_i1.begin(), deq_i1.end()),
//         cycles,
//         "deque_int constructor range"
//     );

//     TEST_CONSTRUCTOR (
//         _deque_string d(deq_s1.begin(), deq_s1.end()),
//         cycles,
//         "deque_string constructor range"
//     );

//     size_t deq_i1_size = deq_i1.size();
//     size_t deq_s1_size = deq_s1.size();
    
//     TEST_ELEM_ACCESS (
//         deq_i1[deq_i1_size/2],
//         cycles,
//         "deque_int operator[halfway]"
//     );

//     TEST_ELEM_ACCESS (
//         deq_i1.front(),
//         cycles,
//         "deque_int front()"
//     );

//     TEST_ELEM_ACCESS (
//         deq_i1.back(),
//         cycles,
//         "deque_int back()"
//     );
//     TEST_ELEM_ACCESS (
//         deq_s1[deq_s1_size/2],
//         cycles,
//         "deque_string operator[halfway]"
//     );

//     TEST_ELEM_ACCESS (
//         deq_s1.front(),
//         cycles,
//         "deque_string front()"
//     );

//     TEST_ELEM_ACCESS (
//         deq_s1.back(),
//         cycles,
//         "deque_string back()"
//     );


//     TEST_PUSH_FRONT (
//         deq_i1,
//         0,
//         cycles, 
//         100, 
//         "deque_int push_front()"
//     ); 
    
//     TEST_PUSH_FRONT (
//         deq_s1,
//         "boo",
//         cycles, 
//         100, 
//         "deque_string push_front()"
//     ); 

//     TEST_PUSH_BACK (
//         deq_i1,
//         0,
//         cycles, 
//         100, 
//         "deque_int push_back()"
//     ); 
    
//     TEST_PUSH_BACK (
//         deq_s1,
//         "boo",
//         cycles, 
//         100, 
//         "deque_string push_back()"
//     ); 

//     TEST_DEQUE_INSERT (
//         deq_i1,
//         -22,
//         (deq_i1_size/2),
//         cycles,
//         100,
//         "deque_int insert middle"
//     );

//     TEST_DEQUE_INSERT (
//         deq_s1,
//         "tja",
//         (deq_s1_size/2),
//         cycles,
//         100,
//         "deque_string insert middle"
//     );

//     TEST_DEQUE_INSERT_RANGE (
//         deq_i1,
//         deq_i2,
//         (deq_i1.begin() + deq_i1_size / 2),
//         cycles, 
//         100,
//         "deque_int insert range"
//     );

//     TEST_DEQUE_INSERT_RANGE (
//         deq_s1,
//         deq_s2,
//         (deq_s1.begin() + deq_s1_size / 2),
//         cycles, 
//         100,
//         "deque_string insert range"
//     );
    
//     TEST_DEQUE_ERASE (
//         deq_i1,
//         (deq_i1_size / 4),
//         cycles, 
//         100,
//         "deque_int erase"
//     );




// }




