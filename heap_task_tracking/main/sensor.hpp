#pragma once

#include <stdio.h>
#include <ctime>

#include <string>
#include <deque>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_random.h"

#define MAX_BUFFER_ENTRIES 64

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
    uint8_t data_size;
    time_t timestamp; 
    std::string data;
}measurement_data_t;

typedef std::deque<measurement_data_t> buffer_t;

typedef struct 
{
    sensor_id_t id;
    buffer_t buf;
    SemaphoreHandle_t s_lock;    
}sensor_t;

inline std::string generate_fake_payload(const char* prefix, size_t len) {
    std::string s = prefix;
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
        measurement_data_t data;
        data.timestamp = time(nullptr); 
        data.data = generate_fake_payload(label, 128);
        data.data_size = data.data.size();
        data.sensor_id = static_cast<sensor_id_t>(s.id); 

        if (xSemaphoreTake(s.s_lock, portMAX_DELAY)) 
        {
            if (s.buf.size() >= MAX_BUFFER_ENTRIES) 
                s.buf.pop_front();

            s.buf.push_back(std::move(data));
            printf("Pushing to %s buffer\n", label);
            xSemaphoreGive(s.s_lock);
        }

        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }
}