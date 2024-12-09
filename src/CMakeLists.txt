add_executable(nano
    nano.cpp
    infrared_sensor.c
    hc_sr04.cpp
    ultrasonic.cpp
    motor.cpp
    servo.cpp
    microphone.cpp
    infrared_sensor.cpp
)

pico_set_program_name(nano "nano")
pico_set_program_version(nano "0.1")

target_include_directories(nano PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../
)

pico_generate_pio_header(nano ${CMAKE_CURRENT_LIST_DIR}/../blink.pio)

pico_enable_stdio_uart(nano 1)
pico_enable_stdio_usb(nano 1)

target_link_libraries(nano
    pico_stdlib
    hardware_spi
    hardware_i2c
    hardware_dma
    hardware_pio
    hardware_interp
    hardware_timer
    hardware_watchdog
    hardware_clocks
    hardware_pwm
    hardware_gpio
)

pico_add_extra_outputs(nano)