#include <stdio.h>
#include "logic_gate.h"
#include "esp_log.h"
#include "lcd_i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// ===== GPIO =====
#define LED_R GPIO_NUM_25
#define LED_G GPIO_NUM_26
#define LED_B GPIO_NUM_27

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
    Lcd_print(0, 0, " ");
    Lcd_print(0, 1, " ");
    VTaskDelay(500 / portTICK_PERIOD_MS);
    Lcd_print(1, 0, "E Logic Board");
    Lcd_print(0, 1, "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -"); 
    vTaskDelay(pdMS_TO_TICKS(500));
}


void app_main(void)
{
    setup();
}

