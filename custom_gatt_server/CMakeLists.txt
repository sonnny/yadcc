cmake_minimum_required(VERSION 3.13)
set(PICO_BOARD "pico_w")
include("/opt/pico-sdk/external/pico_sdk_import.cmake")
project(gatt_server C CXX ASM)
pico_sdk_init()
add_executable(gatt_server main.c)
target_link_libraries(gatt_server
  pico_stdlib
  pico_multicore
  pico_btstack_ble
  pico_btstack_cyw43
  pico_cyw43_arch_none
  )
add_compile_options(-O2)
target_include_directories(gatt_server PUBLIC
    ${CMAKE_CURRENT_LIST_DIR} # For btstack config
    )
pico_btstack_make_gatt_header(gatt_server PRIVATE "${CMAKE_CURRENT_LIST_DIR}/service.gatt")

pico_add_extra_outputs(gatt_server)
