// nano.cpp

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include "motor.h"
#include "ultrasonic.h"
#include "servo.h"
#include "infrared_sensor.h"
#include "microphone.h"

// GPIO pin definitions
#define MOTOR_ENABLE_PIN 21
#define MOTOR_IN1_PIN 20
#define MOTOR_IN2_PIN 18
// Second motor GPIO pins
#define MOTOR2_ENABLE_PIN 26
#define MOTOR2_IN1_PIN 27
#define MOTOR2_IN2_PIN 28
#define SERVO_PIN 5
#define IR_SENSOR_PIN 15
#define MICROPHONE_PIN 13

// Task handles
static TaskHandle_t xControlTaskHandle = NULL;
static TaskHandle_t xMicrophoneTaskHandle = NULL;

// Queue handle
static QueueHandle_t xCommandQueue = NULL;

// Command types
typedef enum
{
    CMD_PAUSE,
    CMD_RESUME
} Command_t;

// Device instances
static L298NMotor *motor;
static L298NMotor *motor2;
static MG90SServo *servo;
static DigitalMicrophone *mic;

// Task for motor control and sensor readings
void vControlTask(void *pvParameters)
{
    bool isPaused = false;
    while (1)
    {
        Command_t cmd;

        // Check for commands
        if (xQueueReceive(xCommandQueue, &cmd, 0) == pdTRUE)
        {
            if (cmd == CMD_PAUSE)
            {
                motor->stop();
                motor2->stop();
                servo->setPosition(1500);
                printf("Paused \n");
                printf("---------------------------------------------- \n");
                isPaused = true;
            }
            else if (cmd == CMD_RESUME)
            {
                printf("Resumed \n");
                printf("---------------------------------------------- \n");
                isPaused = false;
            }
        }

        if (!isPaused)
        {
            // Normal operation
            uint32_t distance = Ultrasonic_GetDistance();
            int ir_value = infrared_sensor_read(IR_SENSOR_PIN);
            printf("Distance: %u cm, IR: %d\n", distance, ir_value);

            if (distance < 20)
            {
                // Stop the motors while waiting
                //motor->stop();
                motor2->stop();

                int time = 0;

                printf("Moving the second Conveyor belt motor\n");

                if (distance < 10)
                {
                    // move motor2 for .5 seconds
                    motor2->setDirection(true);
                    motor2->enable();
                    time = 1000;
                    vTaskDelay(pdMS_TO_TICKS(time));
                    motor2->stop();
                }
               

                printf("Distance less than 20cm, waiting for IR sensor...\n");

                // Wait for the IR sensor to turn on
                while (infrared_sensor_read(IR_SENSOR_PIN))
                {
                    vTaskDelay(pdMS_TO_TICKS(100)); // Delay to prevent busy-waiting
                }
                motor->stop();

                vTaskDelay(pdMS_TO_TICKS(250));

                // IR sensor is on, turn the servo left
                servo->setPosition(2500);
                printf("Turning left\n");

                // Optionally wait before resuming normal operation
                vTaskDelay(pdMS_TO_TICKS(2000));

                if (time == 1000)
                {
                    // move motor2 for .05 seconds
                    motor2->setDirection(false);
                    motor2->enable();
                    vTaskDelay(pdMS_TO_TICKS(time));
                    motor2->stop();
                }

                // Return to normal operation
                servo->setPosition(1500);
                motor->enable();
            }
            else
            {
                motor->enable();
                servo->setPosition(1500);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// Task for microphone monitoring
void vMicrophoneTask(void *pvParameters)
{
    Command_t cmd;
    while (1)
    {
        if (!mic->read())
        {
            cmd = CMD_PAUSE;
            xQueueSend(xCommandQueue, &cmd, 0);
            printf("System paused by microphone.\n");

            // Wait for sound to stop
            while ((!mic->read()))
            {
                vTaskDelay(pdMS_TO_TICKS(10));
            }

            cmd = CMD_RESUME;
            xQueueSend(xCommandQueue, &cmd, 0);
            printf("System resumed.\n");
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

int main()
{
    stdio_init_all();

    // Create command queue
    xCommandQueue = xQueueCreate(5, sizeof(Command_t));

    // Initialize devices
    motor = new L298NMotor(MOTOR_ENABLE_PIN, MOTOR_IN1_PIN, MOTOR_IN2_PIN);
    motor2 = new L298NMotor(MOTOR2_ENABLE_PIN, MOTOR2_IN1_PIN, MOTOR2_IN2_PIN);
    servo = new MG90SServo(SERVO_PIN);
    mic = new DigitalMicrophone(MICROPHONE_PIN);

    Ultrasonic_Init();
    infrared_sensor_init(IR_SENSOR_PIN);

    // Set initial motor state
    motor->setDirection(true);
    motor->enable();
    motor2->setDirection(true);
    motor2->disable();
    servo->setPosition(1500);
    // Create tasks
    xTaskCreate(
        vControlTask,
        "Control",
        configMINIMAL_STACK_SIZE,
        NULL,
        2,
        &xControlTaskHandle);

    xTaskCreate(
        vMicrophoneTask,
        "Microphone",
        configMINIMAL_STACK_SIZE,
        NULL,
        3,
        &xMicrophoneTaskHandle);

    vTaskStartScheduler();

    // Should never get here
    while (1)
        ;
    return 0;
}