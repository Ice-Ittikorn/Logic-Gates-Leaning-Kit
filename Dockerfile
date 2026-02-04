# 1) ใช้ ESP-IDF image อย่างเป็นทางการ
FROM espressif/idf:release-v5.5

# 2) ตั้ง working directory ใน container
WORKDIR /workspace

# 3) copy project ทั้งหมดเข้า container
COPY . .

# 4) เปิด shell เป็น default
CMD ["/bin/bash"]