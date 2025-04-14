idf_component_register(SRCS "Benchmarks.cpp" "string/string.cpp"
                    INCLUDE_DIRS "." "string" "common")
if(DEFINED USE_ETL)
    target_compile_definitions(${COMPONENT_LIB} PRIVATE USE_ETL=${USE_ETL})
endif()
