#include "infrared_sensor.h"
#include "pico/stdlib.h"

void infrared_sensor_init(uint32_t gpio_pin) {
    gpio_init(gpio_pin);
    gpio_set_dir(gpio_pin, GPIO_IN);
}

int infrared_sensor_read(uint32_t gpio_pin) {
    return gpio_get(gpio_pin);
}