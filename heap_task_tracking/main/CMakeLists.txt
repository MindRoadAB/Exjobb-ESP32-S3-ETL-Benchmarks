idf_component_register(
    SRCS "heap_task_tracking_main.cpp"
    INCLUDE_DIRS "."
    PRIV_REQUIRES etl
)
if(DEFINED USE_STATIC)
    target_compile_definitions(${COMPONENT_LIB} PRIVATE USE_STATIC=${USE_STATIC})
endif()

if(DEFINED USE_ETL)
    target_compile_definitions(${COMPONENT_LIB} PRIVATE USE_ETL=${USE_ETL})
endif()
