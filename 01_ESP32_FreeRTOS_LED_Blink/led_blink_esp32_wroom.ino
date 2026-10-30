#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_PIN 2

TaskHandle_t ledTaskHandle = NULL;

void toggleLED(void *parameter)
{
    Serial.println("LED Task Started!");

    while (true)
    {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("LED ON");
        vTaskDelay(pdMS_TO_TICKS(500));

        digitalWrite(LED_PIN, LOW);
        Serial.println("LED OFF");
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void setup()
{
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);

    delay(1000);

    BaseType_t result = xTaskCreate(
        toggleLED,
        "LED_Task",
        2048,
        NULL,
        1,
        &ledTaskHandle
    );

    if (result == pdPASS)
    {
        Serial.println("Task Created Successfully!");
    }
    else
    {
        Serial.println("Task Creation Failed!");
    }
}

void loop()
{
    vTaskDelay(pdMS_TO_TICKS(1000));
}