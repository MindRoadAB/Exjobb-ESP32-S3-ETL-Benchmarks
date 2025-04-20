
// #define MEASUREMENTS 5000
// std::array<uint32_t, 128> times{};
// uint32_t sum{};
// int int_placeholder{};

// #if USE_ETL
//     #include <etl/string.h>
//     etl::string<4> str_placeholder{};
// #else
//     #include <string>
//     std::string str_placeholder{};
// #endif

// /** 
//  *  Wrap the given expression in calls to esp_cpu_get_cycle_count() to measure the cycles.
//  *  Default number of iterations is MEASUREMENTS. See CYCLE_GET_COUNT_CUSTOM_ITERS which takes
//  *  a custom number of iterations.
//  *  The expression is executed once before the measurement to account for any setup time.
//  *  The expression is executed MEASUREMENTS times after the first execution to get a more
//  *  accurate measurement.
//  *  The time taken is printed in cycles per iteration with given label.
//  */
// #define CYCLE_GET_COUNT(expr, time, label)                                                  \
//     do {                                                                                    \
//         uint32_t __start = esp_cpu_get_cycle_count();                                       \
//         expr;                                                                               \
//         for (int retries = 0; retries < MEASUREMENTS; retries++) {                          \
//             expr;                                                                           \
//         }                                                                                   \
//         time = esp_cpu_get_cycle_count() - __start;                                         \
//         printf("%s operation Ran %u iterations, @ %lu cycles per iteration\n",              \
//                 label, MEASUREMENTS, (time / MEASUREMENTS));                                \
//     } while (0)

// #define CYCLE_GET_COUNT_MERGE_UNMERGE(type, expr1, expr2, time) \
//     do { \
//         type h = expr1; \
//         type k = expr2; \
//         uint32_t __start = esp_cpu_get_cycle_count(); \
//         h.merge(k); \
//         time = esp_cpu_get_cycle_count() - __start; \
//         h = expr1; \
//         k = expr2; \
//     } while (0)

// #define CYCLE_GET_COUNT_SET_INSERT_ERASE_AFTER(expr1, expr2, time) \
//     do { \
//         uint32_t __start = esp_cpu_get_cycle_count(); \
//         expr1.insert(expr2); \
//         time = esp_cpu_get_cycle_count() - __start; \
//         auto k = expr1.find(expr2); \
//         expr1.erase(k); \
//     } while (0)

// #define CYCLE_GET_COUNT_SET_INSERT_RANGE(expr1, expr2, time) \
//     do { \
//         uint32_t __start = esp_cpu_get_cycle_count(); \
//         expr1.insert(expr2.begin(), expr2.end()); \
//         time = esp_cpu_get_cycle_count() - __start; \
//         for (const auto& k: expr2) {\
//             expr1.erase(k); \
//         } \
//     } while (0)

// #define CYCLE_GET_COUNT_SET_INSERT_HINT_ERASE_AFTER(expr1, iter_hint, expr2, time) \
//     do { \
//         uint32_t __start = esp_cpu_get_cycle_count(); \
//         expr1.insert(iter_hint, expr2); \
//         time = esp_cpu_get_cycle_count() - __start; \
//         expr1.erase(expr1.find(expr2)); \
//     } while (0)

// #define CYCLE_GET_COUNT_MAP_INSERT_ERASE_AFTER(expr1, expr2, time) \
//     do { \
//         uint32_t __start = esp_cpu_get_cycle_count(); \
//         expr1.insert(expr2); \
//         time = esp_cpu_get_cycle_count() - __start; \
//         expr1.erase(expr2.first); \
//     } while (0)

// #define CYCLE_GET_COUNT_MAP_INSERT_HINT_ERASE_AFTER(expr1, iter_hint, expr2, time) \
//     do { \
//         uint32_t __start = esp_cpu_get_cycle_count(); \
//         expr1.insert(iter_hint, expr2); \
//         time = esp_cpu_get_cycle_count() - __start; \
//         expr1.erase(expr2.first); \
//     } while (0)

// #define CYCLE_GET_COUNT_MAP_INSERT_RANGE(expr1, expr2, time) \
//     do { \
//         uint32_t __start = esp_cpu_get_cycle_count(); \
//         expr1.insert(expr2.begin(), expr2.end()); \
//         time = esp_cpu_get_cycle_count() - __start; \
//         for (const auto& [key, _] : expr2) {\
//             expr1.erase(key); \
//         }\
//     } while (0)

// /**
//  * Wrap the given expression in calls to esp_cpu_get_cycle_count() to measure the cycles, 
//  * with the given number of iterations.
//  *  The expression is executed once before the measurement to account for any setup time.
//  *  The expression is executed MEASUREMENTS times after the first execution to get a more
//  *  accurate measurement.
//  *  The time taken is printed in cycles per iteration with given label. 
//  */
//  #define CYCLE_GET_COUNT_CUSTOM_ITERS(expr, time, iters, label)                              \
//     do {                                                                                    \
//         uint32_t __start = esp_cpu_get_cycle_count();                                       \
//         expr;                                                                               \
//         for (int retries = 0; retries < iters; retries++) {                                 \
//             expr;                                                                           \
//         }                                                                                   \
//         time = esp_cpu_get_cycle_count() - __start;                                         \
//         printf("%s operation Ran %u iterations, @ %lu cycles per iteration\n",              \
//                 label, iters, (time / iters));                                              \
//     } while (0)

// /** 
//  *  Wrap the given expression in calls to esp_cpu_get_cycle_count() to measure the cycles, for expressions
//  *  that return a value, cast to void to avoid unused variable warnings.
//  *  Default number of iterations is MEASUREMENTS. See CYCLE_GET_COUNT_CUSTOM_ITERS which takes
//  *  a custom number of iterations.
//  *  The expression is executed once before the measurement to account for any setup time.
//  *  The expression is executed MEASUREMENTS times after the first execution to get a more
//  *  accurate measurement.
//  *  The time taken is printed in cycles per iteration with given label.
//  */
// #define CYCLE_GET_COUNT_VAL(expr, time, label)                                              \
//     do {                                                                                    \
//         (void)expr;                                                                         \
//         uint32_t __start = esp_cpu_get_cycle_count();                                       \
//         for (int retries = 0; retries < MEASUREMENTS; retries++) {                          \
//             (void)expr;                                                                     \
//         }                                                                                   \
//         time = esp_cpu_get_cycle_count() - __start;                                         \
//         printf("%s operation Ran %u iterations, @ %lu cycles per iteration\n",              \
//                 label, MEASUREMENTS, (time / MEASUREMENTS));                                \
//     } while (0)


// #define CYCLE_GET_COUNT_VECTOR_ERASE_VAL_REPLACE_AFTER(expr1, pos, time) \
//     do { \
//         auto val = expr1[pos]; \
//         auto it = expr1.begin() + pos; \
//         uint32_t __start = esp_cpu_get_cycle_count();  \
//         expr1.erase(it);\
//         time = esp_cpu_get_cycle_count() - __start;  \
//         expr1.insert((expr1.begin() + pos), val);\
//     } while (0)

// #define CYCLE_GET_COUNT_VECTOR_INSERT_VAL_REMOVE_AFTER(expr1, pos, val, time) \
//     do { \
//         auto it = expr1.begin() + pos; \
//         uint32_t __start = esp_cpu_get_cycle_count();  \
//         expr1.insert(it, val);\
//         time = esp_cpu_get_cycle_count() - __start;  \
//         expr1.erase(it); \
//     } while (0)

// #define CYCLE_GET_COUNT_MAP_ERASE_INSERT_AFTER(expr1, expr2, time) \
//     do{ \
//         uint32_t __start = esp_cpu_get_cycle_count(); \
//         expr1.erase(expr2.first); \
//         time = esp_cpu_get_cycle_count() - __start; \
//         expr1.insert(expr2); \
//     } while (0)


// /**
//  * Wrap the given expression in calls to esp_timer_get_time() to measure the elapsed time of the
//  * operation in microseconds.
//  */
// #define TIME_TASK(expr, res, label)                                                         \
//     do {                                                                                    \
//         uint64_t start = esp_timer_get_time();                                              \
//         expr;                                                                               \
//         for (int retries = 0; retries < MEASUREMENTS; retries++) {                          \
//             expr;                                                                           \
//         }                                                                                   \
//         uint64_t end = esp_timer_get_time();                                                \
//         printf("%s operation; Ran %u iterations took %llu milliseconds                      \
//             (%llu microseconds per invocation)\n",                                          \
//             label, MEASUREMENTS, (end -start)/1000, (end - start)/MEASUREMENTS);            \
//     } while (0) 


// /**
//  * Wrapper for testing construction operation.
//  */
// #define TEST_CONSTRUCTOR(expr, time, label)     \
//     CYCLE_GET_COUNT(                            \
//         expr,                                   \
//         time,                                   \
//         "constructor " label \
//     );                  \


// /** 
//  * Wrapper for testing element access 
//  */
// #define TEST_ELEM_ACCESS(expr, time, label)     \
//     CYCLE_GET_COUNT_VAL(                        \
//         expr,                                   \
//         time,                                   \
//         "element access " label \
//     );               \

//     /**
//  * Wrapper for testing reverse operation.
//  */
// #define TEST_REVERSE(expr, time, label)         \
//     CYCLE_GET_COUNT(                            \
//         (_reverse(expr.begin(), expr.end())),                                   \
//         time,                                   \
//         "reverse " label \
//     );                      \

//     /**
//  * Wrapper for testing append (or operator+=) operation.
//  */
// #define TEST_APPEND(expr, time, iters, label)          \
//     CYCLE_GET_COUNT_CUSTOM_ITERS(                            \
//         expr,                                   \
//         time,                                   \
//         iters, \
//         "append " label \
//     );                       \

// /**
//  * Wrapper for testing comparison operation.
//  * expr argument should be container.compare(other_container)
//  */
// #define TEST_COMPARE(expr1, expr2, time, label)         \
//     CYCLE_GET_COUNT_VAL(                            \
//         (expr1.compare(expr2)),                                   \
//         time,                                   \
//         "compare " label                        \
//     );                      \

// #define TEST_POP_BACK(expr1, time, iters, label)         \
//     CYCLE_GET_COUNT_CUSTOM_ITERS( \
//         expr1.pop_back(),\
//         time,  \
//         iters, \
//         "pop_back " label \
//     )

// #define TEST_VECTOR_INSERT(expr1, pos, val, time, iters, label)         \
//     auto it_v_insert = expr1.begin() + pos; \
//     expr1.insert(it_v_insert, val); \
//     expr1.erase(it_v_insert); \
//     for (size_t i = 0; i < iters; i++) { \
//         CYCLE_GET_COUNT_VECTOR_INSERT_VAL_REMOVE_AFTER(                            \
//             expr1,                                   \
//             pos,                                   \
//             val,\
//             time \
//         );                      \
//         times[i] = time; \
//     } \
//     sum = std::accumulate(std::begin(times), std::end(times), 0); \
//     times.fill(0); \
//     printf("insert %s operation Ran %u iterations, @ %lu cycles per iteration\n",              \
//         label, iters, (sum / iters));   

// #define TEST_VECTOR_ERASE(expr1, pos, time, iters, label) \
//     auto val = expr1[pos]; \
//     auto it_v_erase = expr1.begin() + pos; \
//     expr1.erase(it_v_erase); \
//     expr1.insert((expr1.begin() + pos), val);\
//     for (size_t i = 0; i < iters; i++) { \
//         CYCLE_GET_COUNT_VECTOR_ERASE_VAL_REPLACE_AFTER( \
//             expr1, \
//             pos, \
//             time \
//         ); \
//         times[i] = time; \
//     } \
//     sum = std::accumulate(std::begin(times), std::end(times), 0); \
//     times.fill(0); \
//     printf("erase %s operation Ran %u iterations, @ %lu cycles per iteration\n",              \
//         label, iters, (sum / iters));   


// #define TEST_MERGE(type, expr1, expr2, time, iters, label) \
//     for (size_t i = 0; i < iters; i++) { \
//         CYCLE_GET_COUNT_MERGE_UNMERGE( \
//             type, \
//             expr1, \
//             expr2, \
//             time \
//         ); \
//         times[i] = time; \
//     } \
//     sum = std::accumulate(std::begin(times), std::end(times), 0); \
//     times.fill(0); \
//     printf("erase %s operation Ran %u iterations, @ %lu cycles per iteration\n",              \
//         label, iters, (sum / iters));   



// #define TEST_MAP_INSERT(expr1, expr2, time, iters, label) \
//     expr1.insert(expr2); \
//     expr1.erase(expr2.first); \
//     for (size_t i = 0; i < iters; i++) { \
//         CYCLE_GET_COUNT_MAP_INSERT_ERASE_AFTER( \
//             expr1, \
//             expr2, \
//             time \
//         ); \
//         times[i] = time; \
//     } \
//     sum = std::accumulate(std::begin(times), std::end(times), 0); \
//     times.fill(0); \
//     printf("insert %s operation Ran %u iterations, @ %lu cycles per iteration\n",              \
//         label, iters, (sum / iters));  

// #define TEST_MAP_INSERT_HINT(expr1, iter_hint, expr2, time, iters, label) \
//     expr1.insert(iter_hint, expr2); \
//     expr1.erase(expr2.first); \
//     for (size_t i = 0; i < iters; i++) { \
//         CYCLE_GET_COUNT_MAP_INSERT_HINT_ERASE_AFTER( \
//             expr1, \
//             iter_hint, \
//             expr2, \
//             time \
//         ); \
//         times[i] = time; \
//     } \
//     sum = std::accumulate(std::begin(times), std::end(times), 0); \
//     times.fill(0); \
//     printf("insert %s operation Ran %u iterations, @ %lu cycles per iteration\n",              \
//         label, iters, (sum / iters));  


// #define TEST_MAP_INSERT_RANGE(expr1, expr2, time, iters, label) \
//         expr1.insert(expr2.begin(), expr2.end()); \
//         for (const auto& [key, _] : expr2) { \
//             expr1.erase(key); \
//         } \
//         for (size_t i = 0; i < iters; i++) {\
//             CYCLE_GET_COUNT_MAP_INSERT_RANGE( \
//                 expr1, \
//                 expr2, \
//                 time \
//             ); \
//             times[i] = time; \
//         } \
//         sum = std::accumulate(std::begin(times), std::end(times), 0); \
//         times.fill(0); \
//         printf("insert range %s operation Ran %u iterations, @ %lu cycles per iteration\n",              \
//             label, iters, (sum / iters)); 

// /** ASSUME NON-FAILING. We are not testing error-handling, just performance. */
// #define TEST_MAP_ERASE(expr1, expr2, time, iters, label) \
//     expr1.erase(expr2.first); \
//     expr1.insert(expr2); \
//     for (size_t i = 0; i < iters; i++) { \
//         CYCLE_GET_COUNT_MAP_ERASE_INSERT_AFTER( \
//             expr1, \
//             expr2, \
//             time \
//         ); \
//         times[i] = time; \
//     } \
//     sum = std::accumulate(std::begin(times), std::end(times), 0); \
//     times.fill(0); \
//     printf("erase %s operation Ran %u iterations, @ %lu cycles per iteration\n",              \
//         label, iters, (sum / iters));  



// #define TEST_FIND(expr1, expr2, time, label) \
//     CYCLE_GET_COUNT ( \
//         expr1.find(expr2), \
//         time, \
//         "find " label \
//     );


// #define TEST_CONTAINS(expr1, expr2, time, label) \
//     CYCLE_GET_COUNT (\
//         expr1.contains(expr2), \
//         time, \
//         "contains " label \
//     );

// #define TEST_SET_INSERT(expr1, expr2, time, iters, label) \
//     expr1.insert(expr2); \
//     expr1.erase(expr1.find(expr2)); \
//     for (size_t i = 0; i < iters; i++) { \
//         CYCLE_GET_COUNT_SET_INSERT_ERASE_AFTER( \
//             expr1, \
//             expr2, \
//             time \
//         ); \
//         times[i] = time; \
//     } \
//     sum = std::accumulate(std::begin(times), std::end(times), 0); \
//     times.fill(0); \
//     printf("insert %s operation Ran %u iterations, @ %lu cycles per iteration\n",              \
//         label, iters, (sum / iters));  

// #define TEST_SET_INSERT_RANGE(expr1, expr2, time, iters, label) \
//         expr1.insert(expr2.begin(), expr2.end()); \
//         for (const auto& k : expr2) {\
//             expr1.erase(k); \
//         } \
//         for (size_t i = 0; i < iters; i++) {\
//             CYCLE_GET_COUNT_SET_INSERT_RANGE ( \
//                 expr1, \
//                 expr2, \
//                 time \
//             ); \
//             times[i] = time; \
//         }\
//     sum = std::accumulate(std::begin(times), std::end(times), 0); \
//     times.fill(0); \
//     printf("insert %s operation Ran %u iterations, @ %lu cycles per iteration\n",              \
//         label, iters, (sum / iters));  

// #define TEST_SET_INSERT_HINT(expr1, iter_hint, expr2, time, iters, label) \
//     expr1.insert(iter_hint, expr2); \
//     expr1.erase(expr1.find(expr2)); \
//     for (size_t i = 0; i < iters; i++) { \
//         CYCLE_GET_COUNT_SET_INSERT_HINT_ERASE_AFTER( \
//             expr1, \
//             iter_hint, \
//             expr2, \
//             time \
//         ); \
//         times[i] = time; \
//     } \
//     sum = std::accumulate(std::begin(times), std::end(times), 0); \
//     times.fill(0); \
//     printf("insert %s operation Ran %u iterations, @ %lu cycles per iteration\n",              \
//         label, iters, (sum / iters));  

// #define TEST_SET_ERASE(expr1, expr2, time, iters, label) \
//     for (size_t i = 0; i < iters; i++) {\
//         do {\
//             auto val = expr1.find(expr2); \
//             uint32_t __start = esp_cpu_get_cycle_count(); \
//             expr1.erase(val); \
//             time = esp_cpu_get_cycle_count() - __start; \
//             expr1.insert(expr2); \
//         } while (0); \
//         times[i] = time; \
//     } \
//     sum = std::accumulate(std::begin(times), std::end(times), 0); \
//     times.fill(0); \
//     printf("erase %s operation Ran %u iterations, @ %lu cycles per iteration\n",              \
//         label, iters, (sum / iters)); \


// #define TEST_PUSH_FRONT(expr, val, time, iters, label) \
//     expr.push_front(val); \
//     expr.pop_front();\
//     for (size_t i = 0; i < iters; i++) { \
//         do { \
//             uint32_t __start = esp_cpu_get_cycle_count(); \
//             expr.push_front(val); \
//             time = esp_cpu_get_cycle_count() - __start; \
//             expr.pop_front(); \
//         } while (0); \
//         times[i] = time; \
//     } \
//     sum = std::accumulate(std::begin(times), std::end(times), 0); \
//     times.fill(0); \
//     printf("push_front %s operation Ran %u iterations, @ %lu cycles per iteration\n",              \
//         label, iters, (sum / iters)); \

// #define TEST_PUSH_BACK(expr, val, time, iters, label) \
//     expr.push_back(val); \
//     expr.pop_back();\
//     for (size_t i = 0; i < iters; i++) { \
//         do { \
//             uint32_t __start = esp_cpu_get_cycle_count(); \
//             expr.push_back(val); \
//             time = esp_cpu_get_cycle_count() - __start; \
//             expr.pop_back(); \
//         } while (0); \
//         times[i] = time; \
//     } \
//     sum = std::accumulate(std::begin(times), std::end(times), 0); \
//     times.fill(0); \
//     printf("push_back %s operation Ran %u iterations, @ %lu cycles per iteration\n",              \
//         label, iters, (sum / iters)); \

// #define TEST_DEQUE_INSERT(expr, val, pos, time, iters, label) \
//     expr.insert((expr.begin() + pos), val); \
//     expr.erase(expr.begin() + pos); \
//     for (size_t i = 0; i < iters; i++) { \
//         do { \
//             uint32_t __start = esp_cpu_get_cycle_count(); \
//             expr.insert((expr.begin() + pos), val); \
//             time = esp_cpu_get_cycle_count() - __start; \
//             expr.erase(expr.begin() + pos); \
//         } while (0); \
//         times[i] = time; \
//     } \
//     sum = std::accumulate(std::begin(times), std::end(times), 0); \
//     times.fill(0); \
//     printf("insert %s operation Ran %u iterations, @ %lu cycles per iteration\n",              \
//         label, iters, (sum / iters)); \

// #define TEST_DEQUE_INSERT_RANGE(expr1, expr2, pos, time, iters, label) \
//         expr1.insert(pos, expr2.begin(), expr2.end()); \
//         expr1.erase(pos, pos + std::distance(expr2.begin(), expr2.end())); \
//         for (size_t i = 0; i < iters; i++) {\
//             do {\
//                 uint32_t __start = esp_cpu_get_cycle_count(); \
//                 expr1.insert(pos, expr2.begin(), expr2.end()); \
//                 time = esp_cpu_get_cycle_count() - __start; \
//                 expr1.erase(pos, pos + std::distance(expr2.begin(), expr2.end())); \
//             } while (0); \
//             times[i] = time; \
//         } \
//     sum = std::accumulate(std::begin(times), std::end(times), 0); \
//     times.fill(0); \
//     printf("insert %s operation Ran %u iterations, @ %lu cycles per iteration\n",              \
//         label, iters, (sum / iters)); \

// #define TEST_DEQUE_ERASE(expr, pos, time, iters, label) \
//         for (size_t i = 0; i < iters; i++) {\
//             do {\
//                 auto val = expr[pos]; \
//                 uint32_t __start = esp_cpu_get_cycle_count(); \
//                 expr.erase(expr.begin() + pos); \
//                 time = esp_cpu_get_cycle_count() - __start; \
//                 expr.insert(expr.begin() + pos, val); \
//             } while (0); \
//             times[i] = time; \
//         } \
//         sum = std::accumulate(std::begin(times), std::end(times), 0); \
//         times.fill(0); \
//         printf("erase %s operation Ran %u iterations, @ %lu cycles per iteration\n",              \
//             label, iters, (sum / iters)); \












