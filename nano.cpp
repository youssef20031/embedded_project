// nano.cpp

#include "pico/stdlib.h"
#include <stdio.h>
#include "motor.h"
#include "ultrasonic.h"
#include "servo.h"
#include "infrared_sensor.h"
#include "microphone.h"

// GPIO pin definitions
#define MOTOR1_ENABLE_PIN 21
#define MOTOR1_IN1_PIN 20
#define MOTOR1_IN2_PIN 19

#define MOTOR2_ENABLE_PIN 18
#define MOTOR2_IN1_PIN 17
#define MOTOR2_IN2_PIN 16

#define SERVO_PIN 5
#define IR_SENSOR_PIN 6
#define MICROPHONE_PIN 25

// Flag to indicate if the system should pause
volatile bool system_pause = false;

// Interrupt handler for the microphone
void microphone_isr(uint gpio, uint32_t events)
{
    if (events & GPIO_IRQ_EDGE_RISE)
    {
        system_pause = true; // Pause the system when sound is detected
        printf("System paused by microphone interrupt.\n");
    }
    if (events & GPIO_IRQ_EDGE_FALL)
    {
        system_pause = false; // Resume the system when sound stops
        printf("System resumed.\n");
    }
}

int main()
{
    stdio_init_all();

    // Initialize devices
    L298NMotor motor1(MOTOR1_ENABLE_PIN, MOTOR1_IN1_PIN, MOTOR1_IN2_PIN);
    L298NMotor motor2(MOTOR2_ENABLE_PIN, MOTOR2_IN1_PIN, MOTOR2_IN2_PIN);
    Ultrasonic_Init();
    MG90SServo servo(SERVO_PIN);
    infrared_sensor_init();
    DigitalMicrophone mic(MICROPHONE_PIN);

    // Set up microphone interrupt
    gpio_set_irq_enabled_with_callback(
        MICROPHONE_PIN,
        GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,
        true,
        &microphone_isr);

    // Start the motors
    motor1.setDirection(true);
    motor1.enable();
    motor2.setDirection(true);
    motor2.enable();
    motor2.stop();

    while (true)
    {
        if (system_pause)
        {
            // Pause the system
            motor1.stop();
            motor2.stop();
            servo.setPosition(1500); // Neutral position (1.5ms pulse width)
        }
        else
        {
            // System running
            uint32_t distance = Ultrasonic_GetDistance();
            int ir_value = infrared_sensor_read();

            if (distance < 20)
            { // Ultrasonic detects an item within 20cm
                motor1.stop();
                motor2.stop();

                if (ir_value)
                {                            // Both sensors detect an item
                    servo.setPosition(2500); // Turn servo left (2.5ms pulse width)
                    sleep_ms(2500);          // Hold position for 2.5 seconds
                }
                else
                {
                    servo.setPosition(500); // Turn servo right (0.5ms pulse width)
                    sleep_ms(5000);         // Hold position for 5 seconds
                }

                sleep_ms(5000); // Wait for 5 seconds

                // Return servo to neutral position
                servo.setPosition(1500); // Neutral position (1.5ms pulse width)
            }
            else
            {
                motor1.enable();
                motor2.enable();
                servo.setPosition(1500); // Servo in neutral position
            }
        }

        sleep_ms(100);
    }

    return 0;
}