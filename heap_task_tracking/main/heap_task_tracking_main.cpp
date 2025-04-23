#include "esp_heap_task_info.h"
#include "esp_log.h"

#include "sensor.hpp"


#define MAX_TASK_NUM 20
#define MAX_BLOCK_NUM 20
#define MAX_DATA_LENGTH 128


static size_t s_prepopulated_num = 0;
static heap_task_totals_t s_totals_arr[MAX_TASK_NUM];
static heap_task_block_t s_block_arr[MAX_BLOCK_NUM];

static sensor_t temp_buffer;
static sensor_t accel_buffer;
static sensor_t light_buffer;
static sensor_t gps_buffer;
static sensor_t rfid_buffer;
static sensor_t wind_sensor;
static sensor_t moisture_sensor;

static void sensor_init(sensor_t *s, sensor_id_t id)
{
    s->id = id;
    s->s_lock = xSemaphoreCreateMutex();
    if (s->s_lock == NULL) {
        ESP_LOGE("sensor_init", "Failed to create mutex");
        abort();
    }
}

static void esp_dump_per_task_heap_info(void)
{
    heap_task_info_params_t heap_info = {0};
    heap_info.caps[0] = MALLOC_CAP_8BIT;
    heap_info.mask[0] = MALLOC_CAP_8BIT;
    heap_info.caps[1] = MALLOC_CAP_32BIT;
    heap_info.mask[1] = MALLOC_CAP_32BIT;
    heap_info.tasks = NULL;
    heap_info.num_tasks = 0;
    heap_info.totals = s_totals_arr;
    heap_info.num_totals = &s_prepopulated_num;
    heap_info.max_totals = MAX_TASK_NUM;
    heap_info.blocks = s_block_arr;
    heap_info.max_blocks = MAX_BLOCK_NUM;

    heap_caps_get_per_task_info(&heap_info);

    for (int i = 0 ; i < *heap_info.num_totals; i++) {
        printf("Task: %s -> CAP_8BIT: %d CAP_32BIT: %d\n",
                heap_info.totals[i].task && (i == 0) ? "main" : heap_info.totals[i].task ? pcTaskGetName(heap_info.totals[i].task) : "Pre-Scheduler allocs",
                heap_info.totals[i].size[0],
                heap_info.totals[i].size[1]);
    }
    printf("\n\n");
   
}


static void temp_sensor_task(void* arg)     { run_sensor_task(temp_buffer, "TEMP", 1000); } 
static void accel_sensor_task(void* arg)    { run_sensor_task(accel_buffer, "ACCEL", 10000); }
static void light_sensor_task(void* arg)    { run_sensor_task(light_buffer, "LIGHT", 1000); }
static void gps_sensor_task(void* arg)      { run_sensor_task(gps_buffer, "GPS", 1000); }
static void rfid_sensor_task(void* arg)     { run_sensor_task(rfid_buffer, "RFID", 1000); }
static void wind_sensor_task(void* arg)     { run_sensor_task(wind_sensor, "WIND", 1000); }
static void moisture_sensor_task(void* arg) { run_sensor_task(moisture_sensor, "MOISTURE", 1000); }

static void transmit_task(void* arg)
{
    while (true) {
        printf("\n[TRANSMIT TASK] Processing buffers...\n");

        auto flush_buffer = [](const char* label, sensor_t& s_buf) {
            if (xSemaphoreTake(s_buf.s_lock, portMAX_DELAY) == pdTRUE)
            {   
                printf("\nFlushing %s buffer (%u items):\n", label, s_buf.buf.size());
                for (const auto& s : s_buf.buf) {
                    printf("%s\n", s.data.c_str());  
                }
                s_buf.buf.clear();
                printf("\n");
            }
            xSemaphoreGive(s_buf.s_lock);
        };

        flush_buffer("TEMP", temp_buffer);
        flush_buffer("ACCEL", accel_buffer);
        flush_buffer("LIGHT", light_buffer);
        flush_buffer("GPS", gps_buffer);
        flush_buffer("RFID", rfid_buffer);

        esp_dump_per_task_heap_info();
        vTaskDelay(pdMS_TO_TICKS(60000)); 
    }
}

static void task_heap_check(void *arg)
{
    uint32_t free_heap_size{0};
    uint32_t minimum_free_heap_size{0};
    uint32_t largest_block{0}; 
    
    const char *TAG = "task_heap_check";
    while (1)
    {
        free_heap_size = heap_caps_get_free_size(MALLOC_CAP_8BIT);
        minimum_free_heap_size = heap_caps_get_minimum_free_size(MALLOC_CAP_8BIT); 
        largest_block = heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);
        printf("\nHEAP\n");
        ESP_LOGI(TAG, "Free heap size: %lu", free_heap_size);
        ESP_LOGI(TAG, "Minimum free heap size: %lu", minimum_free_heap_size); 
        ESP_LOGI(TAG, "Largest free block: %lu", largest_block);
        ESP_LOGI(TAG, "Ratio: %f\n", (float)largest_block / (float)free_heap_size);
        printf("\n");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }

}



extern "C" void app_main(void)
{
    static const char *TAG = "app_main";
    vTaskSetThreadLocalStoragePointer(NULL, 0, (void*)TAG);
    
    sensor_init(&temp_buffer, TEMP_SENSOR);
    sensor_init(&accel_buffer, ACCEL_SENSOR);
    sensor_init(&light_buffer, LIGHT_SENSOR);
    sensor_init(&gps_buffer, GPS_SENSOR);
    sensor_init(&rfid_buffer, RFID_SENSOR);
    sensor_init(&wind_sensor, WIND_SENSOR);
    sensor_init(&moisture_sensor, MOISTURE_SENSOR);

    xTaskCreatePinnedToCore(temp_sensor_task, "temp_sensor", 4096, NULL, 6, NULL, 0);
    xTaskCreatePinnedToCore(accel_sensor_task, "accel_sensor", 4096, NULL, 5, NULL, 0);
    xTaskCreatePinnedToCore(light_sensor_task, "light_sensor", 4096, NULL, 5, NULL, 0);
    xTaskCreatePinnedToCore(gps_sensor_task, "gps_sensor", 4096, NULL, 5, NULL, 0);
    xTaskCreatePinnedToCore(rfid_sensor_task, "rfid_sensor", 4096, NULL, 5, NULL, 0);
    xTaskCreatePinnedToCore(wind_sensor_task, "wind_sensor", 4096, NULL, 5, NULL, 0);
    xTaskCreatePinnedToCore(moisture_sensor_task, "moisture_sensor", 4096, NULL, 5, NULL, 0);
    
    xTaskCreatePinnedToCore(transmit_task, "transmit_task", 8192, NULL, 5, NULL, 1);
    xTaskCreatePinnedToCore(task_heap_check, "task_heap_check", 4096, NULL, 5, NULL, 1);
}









