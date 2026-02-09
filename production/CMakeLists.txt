cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(production)

# ---------- SPIFFS ----------
set(SPIFFS_IMAGE_DIR ${CMAKE_SOURCE_DIR}/spiffs)

spiffs_create_partition_image(
    spiffs
    ${SPIFFS_IMAGE_DIR}
    FLASH_IN_PROJECT
)