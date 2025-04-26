#include "esp_heap_task_info.h"
#include "esp_log.h"

#include "sensor.hpp"


#define MAX_TASK_NUM 20
#define MAX_BLOCK_NUM 20
#define MAX_DATA_LENGTH 128

#define LARGE_TASK_SIZE 8192 
#define SENSOR_TASK_SIZE 2560
#define BASE_TASK_SIZE 1024 

#if USE_STATIC
    StaticTask_t task_sensor_temp;
    StaticTask_t task_sensor_accel;
    StaticTask_t task_sensor_light;
    StaticTask_t task_sensor_gps;
    StaticTask_t task_sensor_rfid;
    StaticTask_t task_wind_buffer;
    StaticTask_t task_moisture_buffer;
    StaticTask_t task_transmit_buffer;
    StaticTask_t task_heap_check_buffer;
    StaticTask_t task_overflow_flush_buffer;
    
    StackType_t task_temp_stack[SENSOR_TASK_SIZE];
    StackType_t task_accel_stack[SENSOR_TASK_SIZE];
    StackType_t task_light_stack[SENSOR_TASK_SIZE];
    StackType_t task_gps_stack[SENSOR_TASK_SIZE];
    StackType_t task_rfid_stack[SENSOR_TASK_SIZE];
    StackType_t task_wind_stack[SENSOR_TASK_SIZE];
    StackType_t task_moisture_stack[SENSOR_TASK_SIZE];
    StackType_t task_transmit_stack[SENSOR_TASK_SIZE + 512];
    StackType_t task_heap_check_stack[SENSOR_TASK_SIZE];
    StackType_t task_overflow_flush_stack[BASE_TASK_SIZE];
#endif

static size_t s_prepopulated_num = 0;
static heap_task_totals_t s_totals_arr[MAX_TASK_NUM];
static heap_task_block_t s_block_arr[MAX_BLOCK_NUM];

static sensor_t sensor_temp;
static sensor_t sensor_accel;
static sensor_t sensor_light;
static sensor_t sensor_gps;
static sensor_t sensor_rfid;
static sensor_t sensor_wind;
static sensor_t sensor_moisture;


static void esp_task_info_dump(void)
{
    constexpr UBaseType_t MAX_TASKS = MAX_TASK_NUM; // Set this safely above your expected task count
    TaskStatus_t task_array[MAX_TASKS];

    uint32_t total_runtime{};
    UBaseType_t num_tasks_returned = uxTaskGetSystemState(task_array, MAX_TASKS, &total_runtime);

    for (UBaseType_t i = 0; i < num_tasks_returned; i++) {
        TaskStatus_t *ts = &task_array[i];
        printf("TASKDUMP: Task: %-16s | Stack High Water: %5lu  | State: %5d  | Priority: %5u\n" ,
                 ts->pcTaskName,
                 ts->usStackHighWaterMark,
                 ts->eCurrentState,
                 ts->uxCurrentPriority);
    }
}

static void esp_per_task_heap_info_dump(void)
{
    heap_task_info_params_t heap_info{};
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

static void sema_init(SemaphoreHandle_t *s_lock)
{
    *s_lock = xSemaphoreCreateMutex();
    if (*s_lock == NULL) {
        ESP_LOGE("sema_init", "Failed to create mutex");
        abort();
    }
}

measurement_data_t data_1{};
measurement_data_t data_2{};
measurement_data_t data_3{};
measurement_data_t data_4{};
measurement_data_t data_5{};
measurement_data_t data_6{};
measurement_data_t data_7{};

/** replace with template or macro */
static void temp_sensor_task(void* arg)     { run_sensor_task(sensor_temp, data_1, "TEMP", 1000); } 
static void accel_sensor_task(void* arg)    { run_sensor_task(sensor_accel, data_2, "ACCEL", 5000); }
static void light_sensor_task(void* arg)    { run_sensor_task(sensor_light, data_3, "LIGHT", 1000); }
static void gps_sensor_task(void* arg)      { run_sensor_task(sensor_gps, data_4, "GPS", 1000); }
static void rfid_sensor_task(void* arg)     { run_sensor_task(sensor_rfid, data_5, "RFID", 1000); }
static void sensor_wind_task(void* arg)     { run_sensor_task(sensor_wind, data_6, "WIND", 1000); }
static void sensor_moisture_task(void* arg) { run_sensor_task(sensor_moisture, data_7, "MOISTURE", 1000); }
static char task_buffer[512] = {0};


static void transmit_task(void* arg)
{
    while (true) {
        printf("\n[TRANSMIT TASK] Processing buffers...\n");

        auto flush_buffer = [](const char* label, sensor_t& s_buf) {
            if (xSemaphoreTake(s_buf.s_lock, portMAX_DELAY) == pdTRUE)
            {   
                for (const auto& s : s_buf.buf) {
                    printf("%s\n", s.payload.c_str());  
                }
                s_buf.buf.clear();
                printf("\n");  
            }
            #if !USE_ETL
                s_buf.buf = buffer_t();
            #endif

            printf("Buffer %s size: %u\n", label, s_buf.buf.size()); 
            xSemaphoreGive(s_buf.s_lock);
        };

        flush_buffer("TEMP", sensor_temp);
        flush_buffer("ACCEL", sensor_accel);
        flush_buffer("LIGHT", sensor_light);
        flush_buffer("GPS", sensor_gps);
        flush_buffer("RFID", sensor_rfid);
        flush_buffer("WIND", sensor_wind);
        flush_buffer("MOISTURE", sensor_moisture);

        esp_per_task_heap_info_dump();
        esp_task_info_dump();
        //vTaskList(task_buffer); 
        //printf("\nTASK LIST:\n%s\n", task_buffer); 
        vTaskDelay(pdMS_TO_TICKS(10000)); 
    }
}

static void task_overflow_flush(void *arg)
{
    while (1)
    {
        if (xSemaphoreTake(overflow_lock, portMAX_DELAY))
        {
            for (const auto& k: _buf_overflow) 
            {
                printf("Overflow: %s\n", k.payload.c_str());
                _buf_overflow.clear();
            }
            
            #if !USE_ETL
                _buf_overflow = buffer_t();
                printf("overflow new size, capacity: %u\n", _buf_overflow.size());
            #endif 
            xSemaphoreGive(overflow_lock);
        }
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
    #if USE_ETL
        printf("USING ETL\n");
    #else
        printf("USING libstdc++\n");
    #endif

    #if USE_STATIC
        printf("USING xTaskCreateStatic...\n"); 
    #else
        printf("USING xTaskCreate...\n");
    #endif

    vTaskDelay(pdMS_TO_TICKS(1000));

    static const char *TAG = "app_main";
    vTaskSetThreadLocalStoragePointer(NULL, 0, (void*)TAG);
    sema_init(&overflow_lock);


    sensor_init(&sensor_temp, S_TEMP);
    sensor_init(&sensor_accel, S_ACCEL);
    sensor_init(&sensor_light, S_LIGHT);
    sensor_init(&sensor_gps, S_GPS);
    sensor_init(&sensor_rfid, S_RFID);
    sensor_init(&sensor_wind, S_WIND);
    sensor_init(&sensor_moisture, S_MOISTURE);

    #if USE_STATIC
        xTaskCreateStaticPinnedToCore(temp_sensor_task, "temp_sensor", SENSOR_TASK_SIZE, NULL, 5, task_temp_stack, &task_sensor_temp, 0);
        xTaskCreateStaticPinnedToCore(accel_sensor_task, "accel_sensor", SENSOR_TASK_SIZE, NULL, 5, task_accel_stack, &task_sensor_accel, 0);
        xTaskCreateStaticPinnedToCore(light_sensor_task, "light_sensor", SENSOR_TASK_SIZE, NULL, 5, task_light_stack, &task_sensor_light, 0);
        xTaskCreateStaticPinnedToCore(gps_sensor_task, "gps_sensor", SENSOR_TASK_SIZE, NULL, 5, task_gps_stack, &task_sensor_gps, 0);
        xTaskCreateStaticPinnedToCore(rfid_sensor_task, "rfid_sensor", SENSOR_TASK_SIZE, NULL, 5, task_rfid_stack, &task_sensor_rfid, 0);
        xTaskCreateStaticPinnedToCore(sensor_wind_task, "sensor_wind", SENSOR_TASK_SIZE, NULL, 5, task_wind_stack, &task_wind_buffer, 0);
        xTaskCreateStaticPinnedToCore(sensor_moisture_task, "sensor_moisture", SENSOR_TASK_SIZE, NULL, 5, task_moisture_stack, &task_moisture_buffer, 0);
        
        xTaskCreateStaticPinnedToCore(transmit_task, "transmit_task", SENSOR_TASK_SIZE + 512, NULL, 5, task_transmit_stack, &task_transmit_buffer, 1);
        xTaskCreateStaticPinnedToCore(task_heap_check, "task_heap_check", SENSOR_TASK_SIZE, NULL, 5, task_heap_check_stack, &task_heap_check_buffer, 0);
        xTaskCreateStaticPinnedToCore(task_overflow_flush, "task_overflow_flush", BASE_TASK_SIZE, NULL, 5, task_overflow_flush_stack, &task_overflow_flush_buffer, 1);
    
    #else
        xTaskCreatePinnedToCore(temp_sensor_task, "temp_sensor", 2560, NULL, 5, NULL, 0);
        xTaskCreatePinnedToCore(accel_sensor_task, "accel_sensor", 2560, NULL, 5, NULL, 0);
        xTaskCreatePinnedToCore(light_sensor_task, "light_sensor", 2560, NULL, 5, NULL, 0);
        xTaskCreatePinnedToCore(gps_sensor_task, "gps_sensor", 2560, NULL, 5, NULL, 0);
        xTaskCreatePinnedToCore(rfid_sensor_task, "rfid_sensor", 2560, NULL, 5, NULL, 0);
        xTaskCreatePinnedToCore(sensor_wind_task, "sensor_wind", 2560, NULL, 5, NULL, 0);
        xTaskCreatePinnedToCore(sensor_moisture_task, "sensor_moisture", 2560, NULL, 5, NULL, 0);
        
        xTaskCreatePinnedToCore(transmit_task, "transmit_task", 3072, NULL, 5, NULL, 1);
        xTaskCreatePinnedToCore(task_heap_check, "task_heap_check", 2560, NULL, 5, NULL, 1);
        xTaskCreatePinnedToCore(task_overflow_flush, "task_overflow_flush", 2048, NULL, 5, NULL, 1);
    #endif

}









