#pragma once

#include <stdio.h>
#include <ctime>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_random.h"

#define MAX_BUFFER_ENTRIES 64 
#define MAX_PAYLOAD_LENGTH 128

#define OVERFLOW_ENTRIES 4*MAX_BUFFER_ENTRIES

#if USE_ETL
    #include <etl/vector.h>
    //#include <etl/deque.h>
    #include <etl/string.h> 
    typedef etl::string<MAX_PAYLOAD_LENGTH> string_t;  
#else
    #include <string>
    #include <vector>
    //#include <deque>
    #include <vector>
    typedef std::string string_t;
#endif

/** Every 'sensor' has an ID */
typedef enum 
{
    S_TEMP = 0,
    S_PRECIP,
    S_LIGHT,
    S_AIR_Q,
    S_BAROM_P,
    S_WIND,
    S_HUMIDITY, 
}sensor_id_t;

/** Every unit of measurement data will have a timestamp for when
 *  it was recorded, the id for the sensor, and the data recorded (payload)
 */
typedef struct 
{
    sensor_id_t sensor_id;
    time_t timestamp;
    string_t payload;
}measurement_data_t;

#if USE_ETL
    typedef etl::vector<measurement_data_t, MAX_BUFFER_ENTRIES> buffer_t; //etl::deque<measurement_data_t, MAX_BUFFER_ENTRIES> buffer_t;
    #define OVERFLOW_ENTRIES 4*MAX_BUFFER_ENTRIES
    etl::vector<measurement_data_t, OVERFLOW_ENTRIES> _buf_overflow;
    //etl::deque<measurement_data_t, OVERFLOW_ENTRIES> _buf_overflow;
#else
    typedef std::vector<measurement_data_t> buffer_t;
    //typedef std::deque<measurement_data_t> buffer_t;
    buffer_t _buf_overflow;
#endif

/** Lock for the global overflow buffer */
SemaphoreHandle_t overflow_lock;

/** A sensor has an id, a buffer to store data and a lock for the buffer */
typedef struct 
{
    sensor_id_t id;
    buffer_t buf;
    SemaphoreHandle_t s_lock;    
}sensor_t;

/** Wrapper for a sensor with a label, to facilitate pinning the sensor to a task */
typedef struct
{
    sensor_t *sensor;
    const char *label;
}sensor_context_t;

/** Initialize a sensor with its pointer and an id */
inline void sensor_init(sensor_t *s, sensor_id_t id)
{
    s->id = id;
    s->s_lock = xSemaphoreCreateMutex();
    if (s->s_lock == NULL) {
        ESP_LOGE("sensor_init", "Failed to create mutex");
        abort();
    }
}


