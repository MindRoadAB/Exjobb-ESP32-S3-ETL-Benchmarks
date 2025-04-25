#pragma once

#include <stdio.h>
#include <ctime>


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

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_random.h"


typedef enum 
{
    TEMP_SENSOR = 0,
    ACCEL_SENSOR,
    LIGHT_SENSOR,
    GPS_SENSOR,
    RFID_SENSOR,
    WIND_SENSOR,
    MOISTURE_SENSOR
}sensor_id_t;

typedef struct 
{
    sensor_id_t sensor_id;
    time_t timestamp;
    string_t payload;
}measurement_data_t;

#if USE_ETL
    typedef etl::deque<measurement_data_t, MAX_BUFFER_ENTRIES> buffer_t;
#else
    typedef std::deque<measurement_data_t> buffer_t;
#endif


buffer_t overflow;
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

inline void run_sensor_task(sensor_t& s, const char* label, unsigned delay_ms) 
{
    
    while (true) 
    {
        measurement_data_t data{};
        data.timestamp = time(nullptr); 
        data.payload = generate_fake_payload(label, MAX_PAYLOAD_LENGTH - 1);
        data.sensor_id = static_cast<sensor_id_t>(s.id); 

        measurement_data_t data_copy; 
        if (xSemaphoreTake(s.s_lock, portMAX_DELAY)) 
        {
            if (s.buf.size() < MAX_BUFFER_ENTRIES)
            {
                //printf("WATERMARK: %d\n", uxTaskGetStackHighWaterMark(NULL));

                s.buf.push_back(std::move(data));
                //printf("Pushing to %s buffer\n", label);
            }
            else
            {
                ESP_LOGI(label, "Buffer full, pushing to overflow\n");
                data_copy = std::move(data); 
            }
            //ESP_LOGI(label, "about to give the lock\n");
            xSemaphoreGive(s.s_lock);
            ESP_LOGI(label, "lock given\n"); 
        }
        if (data_copy.payload.length() > 0 && xSemaphoreTake(overflow_lock, portMAX_DELAY)) 
        {
            ESP_LOGI(label, "Overflow lock taken\n");
            #if USE_ETL
                if (overflow.size() >= MAX_BUFFER_ENTRIES)
                {
                    buffer_t newBuffer = buffer_t(MAX_BUFFER_ENTRIES * 2);
                    for (auto& k : overflow)
                        newBuffer.push_back(std::move(k)); 
                    overflow = std::move(newBuffer); //buffer_t(MAX_BUFFER_ENTRIES * 2); 
                    ESP_LOGI(label, "Overflow buffer resized\n");
                    ESP_LOGI(label, "New size: %u\n", overflow.size());
                }
            #endif 
            overflow.push_back(std::move(data_copy)); 
            ESP_LOGI(label, "Overflow\n");
            printf("\nOverflow size: %u\n", overflow.size());
            xSemaphoreGive(overflow_lock);
        }


        vTaskDelay(pdMS_TO_TICKS(delay_ms));
        ESP_LOGI(label, "Delay done\n");
    }
}