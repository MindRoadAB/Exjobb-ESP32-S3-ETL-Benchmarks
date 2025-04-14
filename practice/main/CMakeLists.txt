idf_component_register(SRCS "string.cpp"
                    INCLUDE_DIRS ".")
if(DEFINED USE_ETL)
    target_compile_definitions(${COMPONENT_LIB} PRIVATE USE_ETL=${USE_ETL})
endif()
