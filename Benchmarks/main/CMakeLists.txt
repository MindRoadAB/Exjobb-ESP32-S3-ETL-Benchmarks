idf_component_register(SRCS "Benchmarks.cpp" "./vector/vector.cpp" "./map/map.cpp" "./set/set.cpp" "./deque/deque.cpp" 
                    INCLUDE_DIRS "." "string" "vector" "map" "common" "set" "deque")
if(DEFINED USE_ETL)
    target_compile_definitions(${COMPONENT_LIB} PRIVATE USE_ETL=${USE_ETL})
endif()
