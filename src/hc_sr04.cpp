// hc_sr04.cpp

#include "hc_sr04.h"

HCSR04::HCSR04(uint trig_pin, uint echo_pin) : trigPin(trig_pin), echoPin(echo_pin) {
    // Initialize Trigger pin as output
    gpio_init(trigPin);
    gpio_set_dir(trigPin, GPIO_OUT);
    gpio_put(trigPin, 0); // Ensure Trigger pin is low

    // Initialize Echo pin as input
    gpio_init(echoPin);
    gpio_set_dir(echoPin, GPIO_IN);
}

void HCSR04::sendTriggerPulse() {
    gpio_put(trigPin, 1);
    sleep_us(10); // 10 microseconds
    gpio_put(trigPin, 0);
}

absolute_time_t HCSR04::waitForEchoHigh() {
    absolute_time_t start_time = get_absolute_time();
    while (gpio_get(echoPin) == 0) {
        if (absolute_time_diff_us(start_time, get_absolute_time()) > 30000) { // 30ms timeout
            return make_timeout_time_ms(0); // Indicate timeout
        }
    }
    return get_absolute_time();
}

absolute_time_t HCSR04::waitForEchoLow(absolute_time_t start_time) {
    absolute_time_t end_time = get_absolute_time();
    while (gpio_get(echoPin) == 1) {
        if (absolute_time_diff_us(start_time, get_absolute_time()) > 30000) { // 30ms timeout
            return make_timeout_time_ms(0); // Indicate timeout
        }
        end_time = get_absolute_time();
    }
    return end_time;
}

float HCSR04::measureDistanceCM() {
    sendTriggerPulse();

    // Wait for Echo pin to go high
    absolute_time_t start_time = waitForEchoHigh();
    if (absolute_time_diff_us(start_time, start_time) == 0) {
        return -1.0; // Timeout
    }

    // Wait for Echo pin to go low
    absolute_time_t end_time = waitForEchoLow(start_time);
    if (absolute_time_diff_us(start_time, end_time) == 0) {
        return -1.0; // Timeout
    }

    // Calculate pulse duration in microseconds
    uint32_t pulse_duration = absolute_time_diff_us(start_time, end_time);

    // Calculate distance (speed of sound is approximately 0.0343 cm/us)
    float distance = (pulse_duration * 0.0343) / 2;

    return distance;
}