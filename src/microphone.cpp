// digital_microphone.cpp

#include "microphone.h"
#include "pico/stdlib.h"

DigitalMicrophone::DigitalMicrophone(uint32_t pin) : gpio_pin(pin) {
    gpio_init(gpio_pin);
    gpio_set_dir(gpio_pin, GPIO_IN);
}

bool DigitalMicrophone::read() {
    return gpio_get(gpio_pin);
}