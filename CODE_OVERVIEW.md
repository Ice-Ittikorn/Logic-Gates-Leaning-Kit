# ภาพรวมโค้ด E-Logic Board

เอกสารนี้ใช้สรุปโปรเจ็กต์ ESP-IDF ภายใต้โฟลเดอร์ `production/` โดยอธิบายโมดูลหลัก หน้าที่ ข้อมูลที่เปิดเผย และการเชื่อมโยงระหว่างกันเพื่อให้บอร์ด E-Logic ทำงานได้ครบกระบวนการ

## จุดเริ่มต้นของโปรแกรม

- `production/main/main.c`
  - `app_main()` เป็นฟังก์ชันเริ่มต้นของ ESP-IDF เรียก `setup()` แล้วตามด้วย `wifi_ap()`.
  - `setup()` ตั้งค่าไฟสถานะด้วย `led_status_init()` และเตรียมจอ LCD ผ่าน `Lcd_setup()`.
  - `wifi_ap()` เปิดโหมด Access Point (`wifi_ap_start()`), เปิดเว็บเซิร์ฟเวอร์ตั้งค่า (`web_server_start()`), บันทึก SSID/รหัสผ่านที่รับจากฟอร์ม (`g_wifi_ssid`, `g_wifi_pass`) และอัปเดตข้อความบน LCD พร้อมตรวจ `WIFI_CLIENT_IP` เพื่อตรวจจับลูกข่าย.
  - `wifi_sta()` เป็นตัวอย่างการเชื่อม Wi-Fi ภายนอกโดยเรียก `wifi_setup("ssid","password")`.
  - `connect_text()` เป็นงานช่วยแสดงข้อความ “CONNECTING” แบบวิ่งบน LCD และกระพริบ LED RGB ผ่าน `led_status_set()`.

### ตัวแปรที่แชร์ร่วมกัน

| สัญลักษณ์ | ประกาศใน | หน้าที่ |
| --- | --- | --- |
| `lcd_buf[32]` | `main.c` | บัฟเฟอร์เตรียมข้อความก่อนส่งไปยัง LCD |
| `g_wifi_ssid[32]`, `g_wifi_pass[64]` | `components/Wifi_AP/web_server.c` | เก็บข้อมูล Wi-Fi จากฟอร์มในเว็บ |
| `WIFI_CLIENT_IP[16]` | `components/Wifi_AP/wifi_ap.c` | ข้อความ IP ล่าสุดของลูกข่าย AP ใช้ `"0.0.0.0"` เมื่อยังไม่มี |
| `wifi_ap_ip`, `wifi_ap_client_ip` | `components/Wifi_AP/wifi_ap.c` | โครงสร้าง IP ที่ใช้ภายในสำหรับ AP และลูกข่ายล่าสุด |
| `WIFI_STATUS` | `components/Wifi_STA/wifi_sta.c` | สถานะการเชื่อมต่อของโหมด Station (enum `wifi_status_t`) |

## โมดูลเชื่อมต่อฮาร์ดแวร์

### จอ LCD (`components/lcd_i2c`)

- API สาธารณะใน `lcd_i2c.h`: `Lcd_setup()` และ `Lcd_print(position, line, text)`.
- ติดตั้งไดรเวอร์ LCD แบบ I2C (HD44780) บนพอร์ต 0 (`SDA` = 21, `SCL` = 22) `Lcd_setup()` กำหนดค่าทั้งหมดและแสดง splash screen เริ่มต้น ส่วน `Lcd_print()` ตั้งตำแหน่งเคอร์เซอร์แล้วส่งตัวอักษรผ่าน `lcd_data()`.
- `main.c` ใช้ฟังก์ชันนี้เพื่อแสดงข้อความบูต SSID/รหัสผ่าน และคำแนะนำเชื่อมต่อ.

### LED สถานะ RGB (`components/led_status`)

- API สาธารณะ: `led_status_init()` และ `led_status_set(status)`.
- ควบคุม GPIO 19 (แดง), 18 (เขียว), 23 (น้ำเงิน) `led_status_set()` จะปิดไฟทั้งหมดก่อนเปิดเฉพาะสีที่ต้องการ.
- ฟังก์ชัน `connect_text()` สลับ `LED_BLUE` กับ `LED_OFF` เพื่อกระพริบระหว่างรอการเชื่อมต่อ.

## ยูทิลิตีตรรกะ

### ฟังก์ชัน logic gate (`components/logic_gate`)

- ฟังก์ชันเชิงตรรกะพื้นฐาน: `logic_and()`, `logic_or()`, `logic_not()` ทำงานกับค่าจำนวนเต็มแบบ truthy (`0` หรือไม่เป็นศูนย์) และคืนค่าตามตรรกะมาตรฐาน.
- พร้อมใช้งานในงานประมวลผลเพิ่มเติมในอนาคต ปัจจุบันยังไม่มีการเรียกใช้จากส่วนอื่น.

## สแตกเชื่อมต่อเครือข่าย

### โหมด Access Point (`components/Wifi_AP`)

- ค่าคงที่ `ESP_AP_SSID` (`"ESP_0001"`) และ `ESP_AP_PASS` (`"12345678"`) อยู่ใน `wifi_ap.h`.
- `wifi_ap_start()` ดูแลการเปิด AP เต็มชุด:
  - เตรียม NVS, network interface และ event loop.
  - ลงทะเบียน `wifi_ap_event_handler()` สำหรับเหตุการณ์ Wi-Fi และ IP.
  - ตั้งค่า AP (`wifi_config_t`) แล้วเริ่ม Wi-Fi driver.
  - อ่านและเก็บ IP ของ AP ไว้ใน `wifi_ap_ip`.
- `wifi_ap_event_handler()`:
  - เมื่อเกิด `IP_EVENT_AP_STAIPASSIGNED` แปลง IP ของลูกข่ายเป็นข้อความเก็บใน `WIFI_CLIENT_IP`.
  - เมื่อเกิด `WIFI_EVENT_AP_STADISCONNECTED` เคลียร์ `WIFI_CLIENT_IP` และตั้งค่า `wifi_ap_client_ip` ให้เป็นศูนย์.
- `main.c` ใช้ตัวแปรส่วนกลางเหล่านี้เพื่อบันทึก log และแสดงผลบน LCD.

### เว็บเซิร์ฟเวอร์ฝังตัว (`components/Wifi_AP/web_server.c`)

- ทำหน้าที่เป็นพอร์ทัลตั้งค่าแบบง่าย:
  - `web_server_start()` เมาท์ SPIFFS, เริ่ม HTTP server และลงทะเบียน handler ดังนี้
    - `GET /` → ส่งไฟล์ `/spiffs/from.html`
    - `GET /style.css` → ส่ง stylesheet
    - `POST /Success` → ดึงข้อมูลฟอร์มเก็บใน `g_wifi_ssid` / `g_wifi_pass` แล้วตอบกลับด้วย `/spiffs/success.html`
- `send_file()` จัดการอ่านไฟล์จาก SPIFFS ส่งกลับพร้อม MIME type ที่ถูกต้องและตอบ 404 เมื่อหาไฟล์ไม่พบ.
- ข้อมูล SSID/รหัสผ่านที่จัดเก็บไว้สามารถนำไปใช้จากโมดูลอื่นได้ เช่น การแสดงบน LCD หรือเตรียมเชื่อมโหมด Station.

### โหมด Station (`components/Wifi_STA`)

- `wifi_setup(ssid, password)` ตั้งค่าให้อุปกรณ์เป็นลูกข่าย:
  - เริ่มใช้งาน network stack, ลงทะเบียน event handler, คัดลอกข้อมูลไปยัง `wifi_config_t` แล้วเริ่มโหมด Station.
- `wifi_event_handler()` อัปเดต enum `WIFI_STATUS` ตามเหตุการณ์:
  - `WIFI_STATUS_CONNECTING`, `WIFI_STATUS_SUCCESS` รวมถึงสถานะผิดพลาดเมื่อรหัสผ่านผิดหรือหา SSID ไม่เจอ.
- อนาคตสามารถใช้ร่วมกับข้อมูลที่รับผ่านโหมด AP เพื่อเชื่อมต่อเครือข่ายภายนอก.

## ทรัพยากรในระบบไฟล์ (`production/spiffs`)

- `from.html`: ฟอร์ม HTML สำหรับกรอก SSID และรหัสผ่าน.
- `style.css`: ไฟล์ตกแต่งหน้าฟอร์ม.
- `success.html`: หน้ายืนยันว่ารับข้อมูลเรียบร้อย.
- ไฟล์ทั้งหมดจะถูกแพ็กลง SPIFFS และเสิร์ฟผ่านเว็บเซิร์ฟเวอร์ ต้องสร้างอิมเมจด้วย `idf.py spiffsgen` และแฟลชด้วย `idf.py flash`.

## หมายเหตุการ Build และ Flash

- เรียกใช้งานสภาพแวดล้อม ESP-IDF: `source ~/store/Logic/v5.5.2/esp-idf/export.sh`.
- คำสั่ง build/flash ที่ใช้บ่อย (บันทึกไว้ใน `README.md`):
  - `idf.py build`
  - `idf.py flash`
  - `idf.py spiffsgen`
  - `idf.py flash monitor`

## แนวทางขยายต่อ

- หากต้องการใช้ข้อมูล Wi-Fi ที่รับมา สามารถเรียก `wifi_setup(g_wifi_ssid, g_wifi_pass)` หลังตรวจสอบความถูกต้อง พร้อมเสริมการตอบสนองบน LCD/LED.
- โมดูลฮาร์ดแวร์หรือฟังก์ชัน logic เพิ่มเติมสามารถเพิ่มใน `production/components/` แล้วประกาศ header ให้ถูกใช้งานซ้ำได้ง่าย.
