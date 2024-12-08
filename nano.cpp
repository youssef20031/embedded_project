// nano.cpp

#include "pico/stdlib.h"
#include <stdio.h>
#include "motor.h"
#include "ultrasonic.h"
#include "servo.h"
#include "infrared_sensor.h"
#include "microphone.h"

// GPIO pin definitions
#define MOTOR_ENABLE_PIN 2
#define MOTOR_IN1_PIN 3
#define MOTOR_IN2_PIN 4
#define SERVO_PIN 5
#define IR_SENSOR_PIN 6
#define MICROPHONE_PIN 7

// Flag to indicate if the system should pause
volatile bool system_pause = false;

// Interrupt handler for the microphone
void microphone_isr(uint gpio, uint32_t events) {
    if (events & GPIO_IRQ_EDGE_RISE) {
        system_pause = true;  // Pause the system when sound is detected
        printf("System paused by microphone interrupt.\n");
    }
    if (events & GPIO_IRQ_EDGE_FALL) {
        system_pause = false; // Resume the system when sound stops
        printf("System resumed.\n");
    }
}

int main() {
    stdio_init_all();

    // Initialize devices
    L298NMotor motor(MOTOR_ENABLE_PIN, MOTOR_IN1_PIN, MOTOR_IN2_PIN);
    Ultrasonic_Init();
    SG90Servo servo(SERVO_PIN);
    infrared_sensor_init();
    DigitalMicrophone mic(MICROPHONE_PIN);

    // Set up microphone interrupt for both rising and falling edges
    gpio_set_irq_enabled_with_callback(MICROPHONE_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &microphone_isr);

    // Start the motor
    motor.setDirection(true);
    motor.enable();

    while (true) {
        if (system_pause) {
            // Pause the system
            motor.stop();
            servo.setPosition(512); // Neutral position
        } else {
            // System running
            uint32_t distance = Ultrasonic_GetDistance();
            int ir_value = infrared_sensor_read();

            if (distance < 20) { // Ultrasonic detects an item within 20cm
                motor.stop();
                if (ir_value) { // Both ultrasonic and infrared detect an item
                    servo.setPosition(1024); // Turn servo left
                } else {
                    servo.setPosition(0); // Turn servo right
                }
            } else {
                motor.enable();
                servo.setPosition(512); // Servo in neutral position
            }
        }

        sleep_ms(100);
    }

    return 0;
}