// servo.h

#ifndef SERVO_H
#define SERVO_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <stdint.h>
#include <stdio.h>

class SG90Servo {
private:
    uint32_t gpio_pin;
    uint pwm_slice;
    uint pwm_channel;
    uint32_t wrap; // Store wrap value to avoid using pwm_get_wrap()

public:
    // Constructor to initialize the servo on a specific GPIO pin
    SG90Servo(uint32_t pin);

    // Method to set the servo position (0 to 1024)
    void setPosition(uint32_t position);
};

#endif // SERVO_H