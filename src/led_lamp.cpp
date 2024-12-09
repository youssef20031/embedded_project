// led_lamp.cpp

#include "led_lamp.h"
#include "pico/stdlib.h"

LEDLamp::LEDLamp(uint32_t pin) : gpio_pin(pin) {
    gpio_init(gpio_pin);
    gpio_set_dir(gpio_pin, GPIO_OUT);
    gpio_put(gpio_pin, false); // Ensure LED is off initially
}

void LEDLamp::turnOn() {
    gpio_put(gpio_pin, true);
}

void LEDLamp::turnOff() {
    gpio_put(gpio_pin, false);
}