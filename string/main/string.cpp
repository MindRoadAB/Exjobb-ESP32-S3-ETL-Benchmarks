#include <stdio.h>

#ifdef USE_ETL
#include <etl/string.h>
#define MAX_STRLN 30
#else
#include <string>
#endif

using namespace std;

extern "C" void app_main(void)
{
    const char *str_array[] = 
    {
        "abcdefghijklmnopqrstuvwxyz",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
        "abcdefghijklmnopqrstuvwxyzåäö",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZÅÄÖ",
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ",
        "abcdefghijklmnopqrstuvwxyzåäöABCDEFGHIJKLMNOPQRSTUVWXYZÅÄÖ",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzåäö",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZÅÄÖabcdefghijklmnopqrstuvwxyz",
        "0123456789"
    };

    size_t num_strings = sizeof(str_array) / sizeof(str_array[0]);
    #ifdef USE_ETL
    printf("Using ETL...\n");
    etl::string<MAX_STRLN> etl_str_array[num_strings]{};
    #else
    printf("Using libstdc++...\n"); 
    std::string std_str_array[sizeof(str_array) / sizeof(str_array[0])]{};
    #endif

    for (size_t i = 0; i < num_strings; ++i)
    {
        #ifdef USE_ETL 
        etl_str_array[i] = str_array[i];
        printf("%s\n", etl_str_array[i].c_str());
        etl_str_array[i].append(str_array[num_strings - 1]);
        printf("%s\n", etl_str_array[i].c_str());
        if (etl_str_array[i].find('z') != etl::string<MAX_STRLN>::npos)
        {
            printf("Found 'z' in string %zu\n", i);
        }
        else
        {
            printf("'z' not found in string %zu\n", i);
        }
        #else 
        std_str_array[i] = str_array[i];
        printf("%s\n", std_str_array[i].c_str());
        std_str_array[i].append(str_array[num_strings - 1]);
        printf("%s\n", std_str_array[i].c_str());
        if (std_str_array[i].find('z') != std::string::npos)
        {
            printf("Found 'z' in string %zu\n", i);
        }
        else
        {
            printf("'z' not found in string %zu\n", i);
        }
        #endif
    }
}