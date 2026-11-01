
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "esp_log.h"

#define LED_PIN GPIO_NUM_2

static const char *TAG = "RTOS_DEMO";

// Task 1: LED blinking
void ledTask(void *parameter)
{
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    while (1)
    {
        gpio_set_level(LED_PIN, 1);
        ESP_LOGI(TAG, "LED ON");

        vTaskDelay(pdMS_TO_TICKS(500));

        gpio_set_level(LED_PIN, 0);
        ESP_LOGI(TAG, "LED OFF");

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

// Task 2: Serial monitoring
void monitorTask(void *parameter)
{
    while (1)
    {
        ESP_LOGI("MONITOR", "ESP32 RTOS Running");

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// Native ESP-IDF entry point
void app_main(void)
{
    ESP_LOGI(TAG, "Starting Multiple RTOS Tasks");

    xTaskCreate(
        ledTask,
        "LED_Task",
        2048,
        NULL,
        1,
        NULL
    );

    xTaskCreate(
        monitorTask,
        "Monitor_Task",
        2048,
        NULL,
        1,
        NULL
    );
}
