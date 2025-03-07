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
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(nano C CXX ASM)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1


SET(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/FreeRTOS-Kernel)
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

# Add the executable target and specify source files
add_executable(nano
    src/nano.cpp
    src/infrared_sensor.c
    src/hc_sr04.cpp
    src/ultrasonic.cpp
    src/motor.cpp
    src/servo.cpp
    src/microphone.cpp
    # Add other source files if necessary
)

pico_set_program_name(nano "nano")
pico_set_program_version(nano "0.1")

# Generate PIO header
pico_generate_pio_header(nano ${CMAKE_CURRENT_LIST_DIR}/blink.pio)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(nano 1)
pico_enable_stdio_usb(nano 1)

# Add the standard library to the build
target_link_libraries(nano
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(nano PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)

# Add any user requested libraries
target_link_libraries(nano
    pico_stdlib
    hardware_pwm
    hardware_gpio
    hardware_spi
    hardware_i2c
    hardware_dma
    hardware_pio
    hardware_interp
    hardware_timer
    hardware_watchdog
    hardware_clocks
    FreeRTOS-Kernel
    FreeRTOS-Kernel-Heap4
    # Add other required libraries
)

# After including FreeRTOS-Kernel
target_compile_definitions(FreeRTOS-Kernel INTERFACE
    FREERTOS_KERNEL_SMP=0
)

pico_add_extra_outputs(nano)

