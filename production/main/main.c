#include "logic_gate.h"
#include "esp_log.h"
#include "lcd_i2c.h"

void app_main(void)
{
    lcd_init();

    // เหมือน Arduino:
    // lcd.setCursor(0,0); lcd.print("Hello");
    Lcd_print(0, 0, "Hello ESP32");

    // lcd.setCursor(0,1); lcd.print("I2C LCD");
    Lcd_print(0, 1, "I2C LCD");
}