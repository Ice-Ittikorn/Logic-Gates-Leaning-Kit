#include <stdio.h>
#include "logic_gate.h"
#include "esp_log.h"
#include "lcd_i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// ===== GPIO =====
#define LED_R GPIO_NUM_19
#define LED_G GPIO_NUM_18
#define LED_B GPIO_NUM_23

void setup (void)
{
    // GPIO
    gpio_reset_pin(LED_R); // R
    gpio_reset_pin(LED_G); // G
    gpio_reset_pin(LED_B); // B
    gpio_set_direction(LED_R, GPIO_MODE_OUTPUT); 
    gpio_set_direction(LED_G, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_B, GPIO_MODE_OUTPUT);

    // LCD
    lcd_init();
    Lcd_print(0, 0, "                ");
    Lcd_print(0, 1, "                ");
    vTaskDelay(pdMS_TO_TICKS(500));
    Lcd_print(1, 0, "E Logic Board");
    Lcd_print(0, 1, "----------------"); 
    vTaskDelay(pdMS_TO_TICKS(2000));
}


void app_main(void)
{
    setup();
    while (1) {
        // code ที่ต้องการให้วน
        vTaskDelay(pdMS_TO_TICKS(1000));
        Lcd_print(0, 0, "Hello LogicGate!");
        Lcd_print(0, 1, " Welcome to ELK ");
        gpio_set_level(LED_R, 1);
        gpio_set_level(LED_G, 0);
        gpio_set_level(LED_B, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));
        gpio_set_level(LED_R, 0);
        gpio_set_level(LED_G, 1);
        gpio_set_level(LED_B, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));
        gpio_set_level(LED_G, 0);
        gpio_set_level(LED_B, 0);
        gpio_set_level(LED_B, 1);
        vTaskDelay(pdMS_TO_TICKS(1000));

    }
}

