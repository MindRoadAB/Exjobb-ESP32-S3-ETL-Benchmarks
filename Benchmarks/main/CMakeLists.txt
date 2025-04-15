idf_component_register(SRCS "Benchmarks.cpp" 
                    INCLUDE_DIRS "." "string" "vector" "common")
if(DEFINED USE_ETL)
    target_compile_definitions(${COMPONENT_LIB} PRIVATE USE_ETL=${USE_ETL})
endif()
