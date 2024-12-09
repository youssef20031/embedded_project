// motor.cpp

#include "motor.h"
#include <stdio.h>

L298NMotor::L298NMotor(uint enable_pin, uint in1_pin, uint in2_pin)
    : enablePin(enable_pin), in1Pin(in1_pin), in2Pin(in2_pin) {
    
    // Initialize Enable pin
    gpio_init(enablePin);
    gpio_set_dir(enablePin, GPIO_OUT);
    gpio_put(enablePin, 0); // Motor disabled by default

    // Initialize direction pins
    gpio_init(in1Pin);
    gpio_set_dir(in1Pin, GPIO_OUT);
    gpio_put(in1Pin, 0);

    gpio_init(in2Pin);
    gpio_set_dir(in2Pin, GPIO_OUT);
    gpio_put(in2Pin, 0);
}

void L298NMotor::setDirection(bool forward) {
    if (forward) {
        gpio_put(in1Pin, 1);
        gpio_put(in2Pin, 0);
        printf("Motor direction set to FORWARD\n");
    } else {
        gpio_put(in1Pin, 0);
        gpio_put(in2Pin, 1);
        printf("Motor direction set to BACKWARD\n");
    }
}

void L298NMotor::enable() {
    gpio_put(enablePin, 1);
    printf("Motor ENABLED\n");
}

void L298NMotor::disable() {
    gpio_put(enablePin, 0);
    printf("Motor DISABLED\n");
}

void L298NMotor::stop() {
    disable();
}