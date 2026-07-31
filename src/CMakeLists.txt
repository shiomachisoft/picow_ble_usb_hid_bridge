# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(picow_ble_usb_hid_bridge C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        )

# Add executable.  
add_executable(picow_ble_usb_hid_bridge
    main.c
    usb_descriptors.c
    hog_host_demo.c
    picow_bt_example_common.c
    Common.c
    )  
target_link_libraries(picow_ble_usb_hid_bridge
    pico_stdlib
    pico_multicore
    hardware_sync
    pico_unique_id
    tinyusb_device
    tinyusb_board
    pico_btstack_ble
    pico_btstack_classic
    pico_btstack_cyw43
    pico_cyw43_arch_threadsafe_background
    )   
target_include_directories(picow_ble_usb_hid_bridge PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    )
target_compile_definitions(picow_ble_usb_hid_bridge PRIVATE 
    CYW43_LWIP=0
    )    
pico_btstack_make_gatt_header(picow_ble_usb_hid_bridge PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/hog_host_demo.gatt
    )

# disable usb output, enable uart output
pico_enable_stdio_usb(picow_ble_usb_hid_bridge 0)
pico_enable_stdio_uart(picow_ble_usb_hid_bridge 1)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(picow_ble_usb_hid_bridge)
