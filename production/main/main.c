#include <stdio.h>
#include "logic_gate.h"
#include "esp_log.h"
#include "lcd_i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_status.h"


void setup (void)
{
    led_status_init();
    Lcd_setup();
}


void app_main(void)
{
    setup();
    Lcd_print(0, 0, "Hello LogicGate!");
    Lcd_print(0, 1, " Welcome to ELK ");
    
    while (1) {

        led_status_set(LED_RED);
        vTaskDelay(pdMS_TO_TICKS(1000));
        led_status_set(LED_GREEN);
        vTaskDelay(pdMS_TO_TICKS(1000));
        led_status_set(LED_BLUE);
        vTaskDelay(pdMS_TO_TICKS(1000));
        led_status_set(LED_OFF);
    }
}

