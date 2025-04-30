#include "esp_heap_task_info.h"
#include "esp_log.h"

#include "sensor.hpp"


#define MAX_TASK_NUM 20
#define MAX_BLOCK_NUM 20
#define MAX_DATA_LENGTH 128

#define LARGE_TASK_SIZE 8192 
#define SENSOR_TASK_STACK_SIZE 2560
#define BASE_TASK_STACK_SIZE 2048 

#define DELAY_MS_SENSOR_BASE 5000
#define DELAY_MS_SENSOR_FAST 500

#define DELAY_MS_TRANSMIT_BASE 60000
#define DELAY_MS_HEAP_CHECK 5000
#define DELAY_MS_OVERFLOW_FLUSH 60000
#define DELAY_MS_WEATHER_EVENT_ON 30000
#define DELAY_MS_WEATHER_EVENT_OFF 60000 

/** Task tags */
#define TAG_TEMP "TEMP"
#define TAG_PRECIP "PRECIP"
#define TAG_LIGHT "LIGHT"
#define TAG_AIRQ "AIRQ"
#define TAG_BAROMP "BAROMP"
#define TAG_WIND "WIND"
#define TAG_HUMIDITY "HUMIDITY"
#define TAG_TRANSMIT "TRANSMIT"
#define TAG_HEAP_CHECK "HEAP_CHECK"
#define TAG_OVERFLOW_FLUSH "OVERFLOW_FLUSH"
#define TAG_WEATHER_EVENT "EVENT: WEATHER_EVENT"

#define TAG_OVERFLOW "EVENT: OVERFLOW"
#define TAG_RESIZE "EVENT: RESIZE"

#if USE_STATIC
    static StaticTask_t tcb_sensor_temp;
    static StaticTask_t tcb_sensor_precip;
    static StaticTask_t tcb_sensor_light;
    static StaticTask_t tcb_sensor_airq;
    static StaticTask_t task_sensor_baromp;
    static StaticTask_t tcb_sensor_wind;
    static StaticTask_t tcb_sensor_humidity;
    static StaticTask_t tcb_transmit;
    static StaticTask_t tcb_heap_check;
    static StaticTask_t tcb_overflow_flush;
    static StaticTask_t tcb_weather_event;
        
    static StackType_t stack_sensor_task_temp[SENSOR_TASK_STACK_SIZE];
    static StackType_t stack_sensor_task_precip[SENSOR_TASK_STACK_SIZE];
    static StackType_t stack_sensor_task_light[SENSOR_TASK_STACK_SIZE];
    static StackType_t stack_sensor_task_airq[SENSOR_TASK_STACK_SIZE];
    static StackType_t stack_sensor_task_baromp[SENSOR_TASK_STACK_SIZE];
    static StackType_t stack_sensor_task_wind[SENSOR_TASK_STACK_SIZE];
    static StackType_t stack_sensor_task_humidity[SENSOR_TASK_STACK_SIZE];
    static StackType_t stack_task_transmit[SENSOR_TASK_STACK_SIZE + 512];
    static StackType_t stack_task_heap_check[SENSOR_TASK_STACK_SIZE];
    static StackType_t stack_task_overflow_flush[BASE_TASK_STACK_SIZE + 512];
    static StackType_t stack_task_weather_event[BASE_TASK_STACK_SIZE];
#endif

static bool fast_mode = false;

static size_t s_prepopulated_num = 0;
static heap_task_totals_t s_totals_arr[MAX_TASK_NUM];
static heap_task_block_t s_block_arr[MAX_BLOCK_NUM];

static sensor_t sensor_temp;
static sensor_t sensor_precip;
static sensor_t sensor_light;
static sensor_t sensor_airq;
static sensor_t sensor_baromp;
static sensor_t sensor_wind;
static sensor_t sensor_humidity;
static sensor_context_t sensor_context_temp;
static sensor_context_t sensor_context_precip;
static sensor_context_t sensor_context_light;
static sensor_context_t sensor_context_airq;
static sensor_context_t sensor_context_baromp;
static sensor_context_t sensor_context_wind;
static sensor_context_t sensor_context_humidity;

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

static string_t generate_fake_payload(const char* prefix, size_t len) {
    string_t s = prefix; 
    s += ": ";
    for (size_t i = 0; i < (esp_random() % len + 1); ++i) {
        s += static_cast<char>((esp_random() % 26) + 65);  // A–Z
    }
    return s;
}

static void run_sensor_task(sensor_t& s, measurement_data_t& data, const char *label) //, unsigned delay_ms) 
{
    while (true) 
    {
        printf("starting run_sensor_task\n");
        
        data.timestamp = time(nullptr); 
        data.payload = generate_fake_payload(label, MAX_PAYLOAD_LENGTH - 1);
        data.sensor_id = static_cast<sensor_id_t>(s.id); 

        bool buffer_full = false;
        
        if (xSemaphoreTake(s.s_lock, portMAX_DELAY)) 
        {
            if (s.buf.size() < MAX_BUFFER_ENTRIES)
            {
                s.buf.push_back(std::move(data));
                printf("Pushing to %s buffer: %s\n", label, data.payload.c_str());
                printf("Buffer %s size: %u\n", label, s.buf.size());
            }
            else
            {
                ESP_LOGW(label, "Buffer full, pushing to overflow\n");
                buffer_full = true;
            }
            xSemaphoreGive(s.s_lock);
        }
        /** If the buffer was full, data was copied and */
        if (buffer_full && xSemaphoreTake(overflow_lock, portMAX_DELAY)) 
        {
            #if USE_ETL
                if (_buf_overflow.size() >= OVERFLOW_ENTRIES)
                {
                    _buf_overflow.pop_front();
                    //ESP_LOGW(TAG_OVERFLOW, "Overflow full, popping front\n");
                    printf("%s Overflow full, popping front\n", TAG_OVERFLOW); 
                    ESP_LOGW(label, "Overflow full, popping front\n");
                }
            #endif 
            _buf_overflow.push_back(std::move(data)); 
            printf("\nOverflow size: %u\n", _buf_overflow.size()); 
            xSemaphoreGive(overflow_lock);
        }

        if (fast_mode) 
            vTaskDelay(pdMS_TO_TICKS(DELAY_MS_SENSOR_FAST));
        else
            vTaskDelay(pdMS_TO_TICKS(DELAY_MS_SENSOR_BASE));
    }
}

static void sensor_task(void* arg)     
{ 
    measurement_data_t data{};
    sensor_context_t *s = (sensor_context_t *)(arg); 
    run_sensor_task(*(s->sensor), data, s->label); 
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
                printf("%s Resizing buffer %s to 0\n", TAG_RESIZE, label); 
            #endif

            printf("Buffer %s size: %u\n", label, s_buf.buf.size()); 
            xSemaphoreGive(s_buf.s_lock);
        };

        flush_buffer(TAG_TEMP, sensor_temp);
        flush_buffer(TAG_PRECIP, sensor_precip);
        flush_buffer(TAG_LIGHT, sensor_light);
        flush_buffer(TAG_AIRQ, sensor_airq);
        flush_buffer(TAG_BAROMP, sensor_baromp);
        flush_buffer(TAG_WIND, sensor_wind);
        flush_buffer(TAG_HUMIDITY, sensor_humidity);

        esp_per_task_heap_info_dump();
        esp_task_info_dump();
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS_TRANSMIT_BASE)); 
    }
}

static void task_overflow_flush(void *arg)
{
    while (1)
    {
        if (xSemaphoreTake(overflow_lock, portMAX_DELAY))
        {
            if (_buf_overflow.size() == 0)
                xSemaphoreGive(overflow_lock);
            else 
            {
                for (const auto& k: _buf_overflow) 
                {
                    ESP_LOGW(TAG_OVERFLOW_FLUSH, "Overflow: %s\n", k.payload.c_str());
                    _buf_overflow.clear();
                }
                
                #if !USE_ETL
                    _buf_overflow = buffer_t();
                    printf("overflow new size, capacity: %u\n", _buf_overflow.size());
                #endif 
                xSemaphoreGive(overflow_lock);
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS_OVERFLOW_FLUSH));
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
        vTaskDelay(pdMS_TO_TICKS(DELAY_MS_HEAP_CHECK));
    }
}

static void task_weather_event(void *arg)
{
    while (1)
    {
        ESP_LOGW("task weather: ", "Fast mode: %s\n", fast_mode ? "ON" : "OFF");
    
        if (fast_mode)
        {
            //ESP_LOGW(TAG_WEATHER_EVENT, "WEATHER EVENT STARTED\n", );
            printf("%s WEATHER EVENT STARTED\n", TAG_WEATHER_EVENT); 
            vTaskDelay(pdMS_TO_TICKS(DELAY_MS_WEATHER_EVENT_ON));
        }
        else
        {
            printf("%s WEATHER EVENT ENDED\n", TAG_WEATHER_EVENT); 
            //ESP_LOGW(TAG_WEATHER_EVENT, "WEATHER EVENT ENDED\n", );
            vTaskDelay(pdMS_TO_TICKS(DELAY_MS_WEATHER_EVENT_OFF));
        } 
        fast_mode = !fast_mode;
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
    sensor_init(&sensor_precip, S_PRECIP);
    sensor_init(&sensor_light, S_LIGHT);
    sensor_init(&sensor_airq, S_AIR_Q);
    sensor_init(&sensor_baromp, S_BAROM_P);
    sensor_init(&sensor_wind, S_WIND);
    sensor_init(&sensor_humidity, S_HUMIDITY);

    sensor_context_temp = {&sensor_temp, TAG_TEMP}; 
    sensor_context_precip = {&sensor_precip, TAG_PRECIP}; 
    sensor_context_light = {&sensor_light, TAG_LIGHT}; 
    sensor_context_airq = {&sensor_airq, TAG_AIRQ}; 
    sensor_context_baromp = {&sensor_baromp, TAG_BAROMP}; 
    sensor_context_wind = {&sensor_wind, TAG_WIND}; 
    sensor_context_humidity = {&sensor_humidity, TAG_HUMIDITY}; 

    #if USE_STATIC
        xTaskCreateStaticPinnedToCore(sensor_task, TAG_TEMP, SENSOR_TASK_STACK_SIZE, &sensor_context_temp, 5, stack_sensor_task_temp, &tcb_sensor_temp, 0);
        xTaskCreateStaticPinnedToCore(sensor_task, TAG_PRECIP, SENSOR_TASK_STACK_SIZE, &sensor_context_precip, 5, stack_sensor_task_precip, &tcb_sensor_precip, 0);
        xTaskCreateStaticPinnedToCore(sensor_task, TAG_LIGHT, SENSOR_TASK_STACK_SIZE, &sensor_context_light, 5, stack_sensor_task_light, &tcb_sensor_light, 0);
        xTaskCreateStaticPinnedToCore(sensor_task, TAG_AIRQ, SENSOR_TASK_STACK_SIZE, &sensor_context_airq, 5, stack_sensor_task_airq, &tcb_sensor_airq, 0);
        xTaskCreateStaticPinnedToCore(sensor_task, TAG_BAROMP, SENSOR_TASK_STACK_SIZE, &sensor_context_baromp, 5, stack_sensor_task_baromp, &task_sensor_baromp, 0);
        xTaskCreateStaticPinnedToCore(sensor_task, TAG_WIND, SENSOR_TASK_STACK_SIZE, &sensor_context_wind, 5, stack_sensor_task_wind, &tcb_sensor_wind, 0);
        xTaskCreateStaticPinnedToCore(sensor_task, TAG_HUMIDITY, SENSOR_TASK_STACK_SIZE, &sensor_context_humidity, 5, stack_sensor_task_humidity, &tcb_sensor_humidity, 0);
        
        xTaskCreateStaticPinnedToCore(transmit_task, TAG_TRANSMIT, SENSOR_TASK_STACK_SIZE + 512, NULL, 5, stack_task_transmit, &tcb_transmit, 1);
        xTaskCreateStaticPinnedToCore(task_heap_check, TAG_HEAP_CHECK, SENSOR_TASK_STACK_SIZE, NULL, 5, stack_task_heap_check, &tcb_heap_check, 1);
        xTaskCreateStaticPinnedToCore(task_overflow_flush, TAG_OVERFLOW_FLUSH, BASE_TASK_STACK_SIZE + 512, NULL, 5, stack_task_overflow_flush, &tcb_overflow_flush, 1);
        
        xTaskCreateStaticPinnedToCore(task_weather_event, TAG_WEATHER_EVENT, BASE_TASK_STACK_SIZE, NULL, 5, stack_task_weather_event, &tcb_weather_event, 1);
    #else
        
        xTaskCreatePinnedToCore(sensor_task, TAG_TEMP, SENSOR_TASK_STACK_SIZE, &sensor_context_temp, 5, NULL, 0);
        xTaskCreatePinnedToCore(sensor_task, TAG_PRECIP, SENSOR_TASK_STACK_SIZE, &sensor_context_precip, 5, NULL, 0);
        xTaskCreatePinnedToCore(sensor_task, TAG_LIGHT, SENSOR_TASK_STACK_SIZE, &sensor_context_light, 5, NULL, 0);
        xTaskCreatePinnedToCore(sensor_task, TAG_AIRQ, SENSOR_TASK_STACK_SIZE, &sensor_context_airq, 5, NULL, 0);
        xTaskCreatePinnedToCore(sensor_task, TAG_BAROMP, SENSOR_TASK_STACK_SIZE, &sensor_context_baromp, 5, NULL, 0);
        xTaskCreatePinnedToCore(sensor_task, TAG_WIND, SENSOR_TASK_STACK_SIZE, &sensor_context_wind, 5, NULL, 0);
        xTaskCreatePinnedToCore(sensor_task, TAG_HUMIDITY, SENSOR_TASK_STACK_SIZE, &sensor_context_humidity, 5, NULL, 0);
        
        xTaskCreatePinnedToCore(transmit_task, TAG_TRANSMIT, SENSOR_TASK_STACK_SIZE + 512, NULL, 5, NULL, 1);
        xTaskCreatePinnedToCore(task_heap_check, TAG_HEAP_CHECK, SENSOR_TASK_STACK_SIZE, NULL, 5, NULL, 1);
        xTaskCreatePinnedToCore(task_overflow_flush, TAG_OVERFLOW_FLUSH, BASE_TASK_STACK_SIZE + 512, NULL, 5, NULL, 1);
        xTaskCreatePinnedToCore(task_weather_event, TAG_WEATHER_EVENT, BASE_TASK_STACK_SIZE, NULL, 5, NULL, 1);
    #endif

}









