#pragma once
#include <stdint.h>

/* เรียกครั้งเดียวตอนเริ่ม */
void lcd_init(void);

/*
 * แสดงข้อความบน LCD
 * position : คอลัมน์ (0-15)
 * line     : บรรทัด (0 หรือ 1)
 * text     : ข้อความ
 */
void Lcd_print(uint8_t position, uint8_t line, const char *text);