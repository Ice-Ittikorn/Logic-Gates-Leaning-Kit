#include "lcd_i2c.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define I2C_PORT   I2C_NUM_0
#define SDA_PIN    21
#define SCL_PIN    22
#define LCD_ADDR   0x27   // ถ้าไม่ขึ้น ลอง 0x3F
#define LCD_COLS   16

/* ===== low level ===== */
static void lcd_write(uint8_t data)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (LCD_ADDR << 1), true);
    i2c_master_write_byte(cmd, data | 0x08, true);   // backlight ON
    i2c_master_write_byte(cmd, data | 0x0C, true);   // EN = 1
    i2c_master_write_byte(cmd, data | 0x08, true);   // EN = 0
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_PORT, cmd, pdMS_TO_TICKS(50));
    i2c_cmd_link_delete(cmd);
}

static void lcd_send(uint8_t value, uint8_t mode)
{
    lcd_write((value & 0xF0) | mode);
    lcd_write(((value << 4) & 0xF0) | mode);
    vTaskDelay(pdMS_TO_TICKS(2));
}

static void lcd_cmd(uint8_t cmd)
{
    lcd_send(cmd, 0x00);
}

static void lcd_data(uint8_t data)
{
    lcd_send(data, 0x01);
}

/* ===== public ===== */
void lcd_init(void)
{
    i2c_config_t cfg = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = SDA_PIN,
        .scl_io_num = SCL_PIN,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000
    };

    i2c_param_config(I2C_PORT, &cfg);
    i2c_driver_install(I2C_PORT, cfg.mode, 0, 0, 0);

    vTaskDelay(pdMS_TO_TICKS(50));

    // init LCD (เหมือน LiquidCrystal_I2C)
    lcd_cmd(0x33);
    lcd_cmd(0x32);
    lcd_cmd(0x28); // 4-bit, 2 line
    lcd_cmd(0x0C); // display ON
    lcd_cmd(0x06);
    lcd_cmd(0x01);
    vTaskDelay(pdMS_TO_TICKS(5));
}

void Lcd_print(uint8_t position, uint8_t line, const char *text)
{
    uint8_t address = (line == 0) ? 0x80 : 0xC0;
    lcd_cmd(address + position);

    while (*text) {
        lcd_data((uint8_t)*text++);
    }
}