#include <stdio.h>
#include <algorithm>
#if USE_ETL
#include <etl/string.h>
#define MAX_STRLN 60 
using _string = etl::string<MAX_STRLN>;
#else
#include <string>
using _string = std::string;
#endif

void string_operations(void)
{

    #if USE_ETL
    printf("Using ETL...\n");
    #else
    printf("Using libstdc++...\n"); 
    #endif
    const char *c_str_array[] = 
    
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

    constexpr size_t num_strings = sizeof(c_str_array) / sizeof(c_str_array[0]);
    _string str_array[num_strings]{};

    for (size_t i = 0; i < num_strings; ++i)
    {
        str_array[i] = c_str_array[i];
        printf("%s\n", str_array[i].c_str());
        str_array[i].append(c_str_array[num_strings - 1]);
        printf("%s\n", str_array[i].c_str());
    }  
    
    _string str, str2 = {""};
    
    char i{65};
    while (i < 123) 
    {
        str += i;
        i++;
    }

    printf("str is %s\n", str.c_str());
    
    std::reverse(str.begin(), str.end());
    printf("str is now: %s\n", str.c_str());

    std::reverse(str.begin(), str.end());
    printf("str is now: %s\n", str.c_str());
    
    std::reverse_copy(std::begin(str), std::end(str), std::begin(str2));
    printf("str2 is: %s\n", str2.c_str());
}

extern "C" void app_main(void)
{
    size_t times{10};
    while (times != 0)
    {
        string_operations();
        times--;
    }
}
