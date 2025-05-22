cmake_minimum_required(VERSION 3.13)
set(PICO_BOARD "pico_w")
include("/opt/pico-sdk/external/pico_sdk_import.cmake")
project(picdcc C CXX ASM)
pico_sdk_init()
add_executable(picdcc main.c dcc.c)
target_link_libraries(picdcc
  pico_stdlib
  pico_multicore
  pico_btstack_ble
  pico_btstack_cyw43
  pico_cyw43_arch_none
  hardware_gpio
  hardware_pio
  hardware_clocks)
#add_compile_options(-O2)
pico_enable_stdio_usb(picdcc 1)
target_include_directories(picdcc 
  PUBLIC
    thread
    ble)
pico_btstack_make_gatt_header(picdcc PRIVATE "${CMAKE_CURRENT_LIST_DIR}/ble/pico_demo_gatt_service.gatt")
pico_generate_pio_header(picdcc ${CMAKE_CURRENT_LIST_DIR}/pulse.pio)
pico_add_extra_outputs(picdcc)
