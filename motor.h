// motor.h

#ifndef MOTOR_H
#define MOTOR_H

#include "pico/stdlib.h"

class L298NMotor {
private:
    uint enablePin;  // GPIO pin for enabling the motor
    uint in1Pin;     // GPIO pin for direction
    uint in2Pin;     // GPIO pin for direction

public:
    // Constructor to initialize the pins
    L298NMotor(uint enable_pin, uint in1_pin, uint in2_pin);

    // Method to set motor direction: true for forward, false for backward
    void setDirection(bool forward);

    // Method to enable the motor
    void enable();

    // Method to disable the motor
    void disable();

    // Method to stop the motor (disable)
    void stop();
};

#endif // MOTOR_H