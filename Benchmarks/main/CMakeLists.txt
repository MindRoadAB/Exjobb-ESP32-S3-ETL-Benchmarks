idf_component_register(SRCS "Benchmarks.cpp" "./vector/vector.cpp" "./map/map.cpp" 
                    INCLUDE_DIRS "." "vector" "map" "common")
if(DEFINED USE_ETL)
    target_compile_definitions(${COMPONENT_LIB} PRIVATE USE_ETL=${USE_ETL})
endif()
