// hc_sr04.h

#ifndef HC_SR04_H
#define HC_SR04_H

#include "pico/stdlib.h"

class HCSR04 {
public:
    /**
     * @brief Constructor to initialize Trigger and Echo pins.
     * 
     * @param trig_pin GPIO pin number for Trigger.
     * @param echo_pin GPIO pin number for Echo.
     */
    HCSR04(uint trig_pin, uint echo_pin);

    /**
     * @brief Measures the distance in centimeters.
     * 
     * @return float Distance measured by the sensor. Returns -1.0 on timeout.
     */
    float measureDistanceCM();

private:
    uint trigPin;  // GPIO pin for Trigger
    uint echoPin;  // GPIO pin for Echo

    /**
     * @brief Sends a trigger pulse to initiate distance measurement.
     */
    void sendTriggerPulse();

    /**
     * @brief Waits for the Echo pin to go high.
     * 
     * @return absolute_time_t Start time when Echo pin goes high.
     */
    absolute_time_t waitForEchoHigh();

    /**
     * @brief Waits for the Echo pin to go low.
     * 
     * @param start_time The time when Echo pin went high.
     * @return absolute_time_t End time when Echo pin goes low.
     */
    absolute_time_t waitForEchoLow(absolute_time_t start_time);
};

#endif // HC_SR04_H