set(SRC_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
set(INC_DIRS "." "string" "vector" "map" "set" "deque" "common")

set(SOURCES
    ${SRC_ROOT}/Benchmarks.cpp
)

if(DEFINED TYPE)
    string(TOUPPER "${TYPE}" TYPE_UPPER)
    message(STATUS "type is: ${TYPE}")
    if(TYPE_UPPER STREQUAL "STRING")
        list(APPEND SOURCES ${SRC_ROOT}/string/string.cpp)
    elseif(TYPE_UPPER STREQUAL "VECTOR")
        list(APPEND SOURCES ${SRC_ROOT}/vector/vector.cpp)
    elseif(TYPE_UPPER STREQUAL "MAP")
        list(APPEND SOURCES ${SRC_ROOT}/map/map.cpp)
    elseif(TYPE_UPPER STREQUAL "SET")
        list(APPEND SOURCES ${SRC_ROOT}/set/set.cpp)
    elseif(TYPE_UPPER STREQUAL "DEQUE")
        list(APPEND SOURCES ${SRC_ROOT}/deque/deque.cpp)
    else()
        message(FATAL_ERROR "Invalid type=${TYPE}")
    endif()
endif()

idf_component_register(
    SRCS "${SOURCES}"
    INCLUDE_DIRS "${INC_DIRS}"
)

if(TYPE_UPPER STREQUAL "STRING")
    target_compile_definitions(${COMPONENT_LIB} PRIVATE USE_STRING=1)
elseif(TYPE_UPPER STREQUAL "VECTOR")
    target_compile_definitions(${COMPONENT_LIB} PRIVATE USE_VECTOR=1)
elseif(TYPE_UPPER STREQUAL "MAP")
    target_compile_definitions(${COMPONENT_LIB} PRIVATE USE_MAP=1)
elseif(TYPE_UPPER STREQUAL "SET")
    target_compile_definitions(${COMPONENT_LIB} PRIVATE USE_SET=1)
elseif(TYPE_UPPER STREQUAL "DEQUE")
    target_compile_definitions(${COMPONENT_LIB} PRIVATE USE_DEQUE=1)
endif()


if(DEFINED USE_ETL)
    target_compile_definitions(${COMPONENT_LIB} PRIVATE USE_ETL=${USE_ETL})
endif()





