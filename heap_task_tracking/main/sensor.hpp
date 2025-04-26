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



inline void sensor_init(sensor_t *s, sensor_id_t id)
{
    s->id = id;
    s->s_lock = xSemaphoreCreateMutex();
    if (s->s_lock == NULL) {
        ESP_LOGE("sensor_init", "Failed to create mutex");
        abort();
    }
}

inline string_t generate_fake_payload(const char* prefix, size_t len) {
    string_t s = prefix; 
    s += ": ";
    for (size_t i = 0; i < (esp_random() % len + 1); ++i) {
        s += static_cast<char>((esp_random() % 26) + 65);  // A–Z
    }
    return s;
}

inline void run_sensor_task(sensor_t& s, measurement_data_t& data, const char* label, unsigned delay_ms) 
{
    
    while (true) 
    {
        printf("starting run_sensor_task\n");
        
        data.timestamp = time(nullptr); 
        data.payload = generate_fake_payload(label, MAX_PAYLOAD_LENGTH - 1);
        data.sensor_id = static_cast<sensor_id_t>(s.id); 

        measurement_data_t data_copy; 
        
        if (xSemaphoreTake(s.s_lock, portMAX_DELAY)) 
        {
            if (s.buf.size() < MAX_BUFFER_ENTRIES)
            {
                s.buf.push_back(std::move(data));
                printf("Pushing to %s buffer: %s\n", label, data.payload.c_str());
            }
            else
            {
                ESP_LOGW(label, "Buffer full, pushing to overflow\n");
                data_copy = std::move(data); 
            }
            xSemaphoreGive(s.s_lock);
        }
        if (data_copy.payload.length() > 0 && xSemaphoreTake(overflow_lock, portMAX_DELAY)) 
        {
            #if USE_ETL
                if (_buf_overflow.size() >= OVERFLOW_ENTRIES)
                {
                    _buf_overflow.pop_front();
                    ESP_LOGW(label, "Overflow full, popping front\n");
                }
            #endif 
            _buf_overflow.push_back(std::move(data_copy)); 
            printf("\nOverflow size: %u\n", _buf_overflow.size());
            xSemaphoreGive(overflow_lock);
        }


        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }
}