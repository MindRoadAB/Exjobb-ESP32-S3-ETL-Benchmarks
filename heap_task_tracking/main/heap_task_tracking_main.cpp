#include "esp_heap_task_info.h"
#include "esp_log.h"

#include "sensor.hpp"


#define MAX_TASK_NUM 20
#define MAX_BLOCK_NUM 20
#define MAX_DATA_LENGTH 128

#define LARGE_TASK_SIZE 8192 
#define SENSOR_TASK_SIZE 2560
#define BASE_TASK_SIZE 2048 

#if USE_STATIC
    static StaticTask_t tcb_sensor_temp;
    static StaticTask_t tcb_sensor_accel;
    static StaticTask_t tcb_sensor_light;
    static StaticTask_t tcb_sensor_gps;
    static StaticTask_t task_sensor_rfid;
    static StaticTask_t tcb_sensor_wind;
    static StaticTask_t tcb_sensor_moisture;
    static StaticTask_t tcb_transmit;
    static StaticTask_t tcb_heap_check;
    static StaticTask_t tcb_overflow_flush;
        
    static StackType_t stack_sensor_task_temp[SENSOR_TASK_SIZE];
    static StackType_t stack_sensor_task_accel[SENSOR_TASK_SIZE];
    static StackType_t stack_sensor_task_light[SENSOR_TASK_SIZE];
    static StackType_t stack_sensor_task_gps[SENSOR_TASK_SIZE];
    static StackType_t stack_sensor_task_rfid[SENSOR_TASK_SIZE];
    static StackType_t stack_sensor_task_wind[SENSOR_TASK_SIZE];
    static StackType_t stack_sensor_task_moisture[SENSOR_TASK_SIZE];
    static StackType_t stack_task_transmit[SENSOR_TASK_SIZE + 512];
    static StackType_t stack_task_heap_check[SENSOR_TASK_SIZE];
    static StackType_t stack_task_overflow_flush[BASE_TASK_SIZE];
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
static sensor_context_t sensor_context_temp;
static sensor_context_t sensor_context_accel;
static sensor_context_t sensor_context_light;
static sensor_context_t sensor_context_gps;
static sensor_context_t sensor_context_rfid;
static sensor_context_t sensor_context_wind;
static sensor_context_t sensor_context_moisture;

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
        printf("HERE\n");
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

static string_t generate_fake_payload(const char* prefix, size_t len) {
    string_t s = prefix; 
    s += ": ";
    for (size_t i = 0; i < (esp_random() % len + 1); ++i) {
        s += static_cast<char>((esp_random() % 26) + 65);  // A–Z
    }
    return s;
}

static void run_sensor_task(sensor_t& s, measurement_data_t& data, const char *label, unsigned delay_ms) 
{
    
    while (true) 
    {
        printf("starting run_sensor_task\n");
        
        data.timestamp = time(nullptr); 
        data.payload = generate_fake_payload(label, MAX_PAYLOAD_LENGTH - 1);
        data.sensor_id = static_cast<sensor_id_t>(s.id); 

        //measurement_data_t data_copy{}; 
        bool buffer_full = false;
        
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
                // data_copy = std::move(data); 
                buffer_full = true;
            }
            xSemaphoreGive(s.s_lock);
        }
        /** If the buffer was full, data was copied and */
        if (/**data_copy.payload.length() > 0*/ buffer_full && xSemaphoreTake(overflow_lock, portMAX_DELAY)) 
        {
            #if USE_ETL
                if (_buf_overflow.size() >= OVERFLOW_ENTRIES)
                {
                    _buf_overflow.pop_front();
                    ESP_LOGW(label, "Overflow full, popping front\n");
                }
            #endif 
            _buf_overflow.push_back(std::move(data)); 
            printf("\nOverflow size: %u\n", _buf_overflow.size());
            xSemaphoreGive(overflow_lock);
        }

        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }
}

static void sensor_task(void* arg)     
{ 
    measurement_data_t data{};
    sensor_context_t *s = (sensor_context_t *)(arg); 
    run_sensor_task(*(s->sensor), data, s->label, s->delay_ms); 
}


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

        // printf("\nFLUSHED\n");
        // // esp_per_task_heap_info_dump();
        // printf("finished per task heap info dump\n"); 
        esp_task_info_dump();
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

    sensor_context_temp = {&sensor_temp, "TEMP", 1000};
    sensor_context_accel = {&sensor_accel, "ACCEL", 1000};
    sensor_context_light = {&sensor_light, "LIGHT", 5000};
    sensor_context_gps = {&sensor_gps, "GPS", 1000};
    sensor_context_rfid = {&sensor_rfid, "RFID", 1000};
    sensor_context_wind = {&sensor_wind, "WIND", 1000};
    sensor_context_moisture = {&sensor_moisture, "MOISTURE", 1000};

    #if USE_STATIC
        xTaskCreateStaticPinnedToCore(sensor_task, "temp_sensor", SENSOR_TASK_SIZE, &sensor_context_temp, 5, stack_sensor_task_temp, &tcb_sensor_temp, 0);
        xTaskCreateStaticPinnedToCore(sensor_task, "accel_sensor", SENSOR_TASK_SIZE, &sensor_context_accel, 5, stack_sensor_task_accel, &tcb_sensor_accel, 0);
        xTaskCreateStaticPinnedToCore(sensor_task, "light_sensor", SENSOR_TASK_SIZE, &sensor_context_light, 5, stack_sensor_task_light, &tcb_sensor_light, 0);
        xTaskCreateStaticPinnedToCore(sensor_task, "gps_sensor", SENSOR_TASK_SIZE, &sensor_context_gps, 5, stack_sensor_task_gps, &tcb_sensor_gps, 0);
        xTaskCreateStaticPinnedToCore(sensor_task, "rfid_sensor", SENSOR_TASK_SIZE, &sensor_context_rfid, 5, stack_sensor_task_rfid, &task_sensor_rfid, 0);
        xTaskCreateStaticPinnedToCore(sensor_task, "wind_sensor", SENSOR_TASK_SIZE, &sensor_context_wind, 5, stack_sensor_task_wind, &tcb_sensor_wind, 0);
        xTaskCreateStaticPinnedToCore(sensor_task, "moisture_sensor", SENSOR_TASK_SIZE, &sensor_context_moisture, 5, stack_sensor_task_moisture, &tcb_sensor_moisture, 0);
        
        xTaskCreateStaticPinnedToCore(transmit_task, "transmit_task", SENSOR_TASK_SIZE + 512, NULL, 5, stack_task_transmit, &tcb_transmit, 1);
        xTaskCreateStaticPinnedToCore(task_heap_check, "task_heap_check", SENSOR_TASK_SIZE, NULL, 5, stack_task_heap_check, &tcb_heap_check, 1);
        xTaskCreateStaticPinnedToCore(task_overflow_flush, "task_overflow_flush", BASE_TASK_SIZE, NULL, 5, stack_task_overflow_flush, &tcb_overflow_flush, 1);
    
    #else
        
        xTaskCreatePinnedToCore(sensor_task, "temp_sensor", 2560, &sensor_context_temp, 5, NULL, 0);
        xTaskCreatePinnedToCore(sensor_task, "accel_sensor", 2560, &sensor_context_accel, 5, NULL, 0);
        xTaskCreatePinnedToCore(sensor_task, "light_sensor", 2560, &sensor_context_light, 5, NULL, 0);
        xTaskCreatePinnedToCore(sensor_task, "gps_sensor", 2560, &sensor_context_gps, 5, NULL, 0);
        xTaskCreatePinnedToCore(sensor_task, "rfid_sensor", 2560, &sensor_context_rfid, 5, NULL, 0);
        xTaskCreatePinnedToCore(sensor_task, "sensor_wind", 2560, &sensor_context_wind, 5, NULL, 0);
        xTaskCreatePinnedToCore(sensor_task, "sensor_moisture", 2560, &sensor_context_moisture, 5, NULL, 0);
        
        xTaskCreatePinnedToCore(transmit_task, "transmit_task", 3072, NULL, 5, NULL, 1);
        xTaskCreatePinnedToCore(task_heap_check, "task_heap_check", 2560, NULL, 5, NULL, 1);
        xTaskCreatePinnedToCore(task_overflow_flush, "task_overflow_flush", 2048, NULL, 5, NULL, 1);
    #endif

}









