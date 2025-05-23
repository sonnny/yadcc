cmake_minimum_required(VERSION 3.13)
set(PICO_BOARD "pico_w")
include("/opt/pico-sdk/external/pico_sdk_import.cmake")
project(gatt_client C CXX ASM)
pico_sdk_init()
add_executable(gatt_client main.c)
target_link_libraries(gatt_client
  pico_stdlib
  pico_multicore
  pico_btstack_ble
  pico_btstack_cyw43
  pico_cyw43_arch_none
  )
pico_enable_stdio_usb(gatt_client 1)
add_compile_options(-O2)
target_include_directories(gatt_client PUBLIC
    ${CMAKE_CURRENT_LIST_DIR} # For btstack config
    )
target_compile_definitions(gatt_client PRIVATE RUNNING_AS_CLIENT=1)
pico_add_extra_outputs(gatt_client)
