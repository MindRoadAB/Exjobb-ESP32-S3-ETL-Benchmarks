#pragma once

#include <stdio.h>
#include <ctime>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_random.h"

#define MAX_BUFFER_ENTRIES 64 
#define MAX_PAYLOAD_LENGTH 128

#if USE_ETL
    #include <etl/deque.h>
    #include <etl/string.h> 
    typedef etl::string<MAX_PAYLOAD_LENGTH> string_t;
#else
    #include <string>
    #include <deque>
    typedef std::string string_t;
#endif

typedef enum 
{
    S_TEMP = 0,
    S_ACCEL,
    S_LIGHT,
    S_GPS,
    S_RFID,
    S_WIND,
    S_MOISTURE, 
}sensor_id_t;

typedef struct 
{
    sensor_id_t sensor_id;
    time_t timestamp;
    string_t payload;
}measurement_data_t;

#if USE_ETL
    typedef etl::deque<measurement_data_t, MAX_BUFFER_ENTRIES> buffer_t;
    #define OVERFLOW_ENTRIES 4*MAX_BUFFER_ENTRIES
    etl::deque<measurement_data_t, OVERFLOW_ENTRIES> _buf_overflow;
#else
    typedef std::deque<measurement_data_t> buffer_t;
    buffer_t _buf_overflow;
#endif

SemaphoreHandle_t overflow_lock;

typedef struct 
{
    sensor_id_t id;
    buffer_t buf;
    SemaphoreHandle_t s_lock;    
}sensor_t;

typedef struct
{
    sensor_t *sensor;
    const char *label;
    unsigned delay_ms;
}sensor_context_t;

inline void sensor_init(sensor_t *s, sensor_id_t id)
{
    s->id = id;
    s->s_lock = xSemaphoreCreateMutex();
    if (s->s_lock == NULL) {
        ESP_LOGE("sensor_init", "Failed to create mutex");
        abort();
    }
}


