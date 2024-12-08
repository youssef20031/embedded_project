#include "infrared_sensor.h"
#include "pico/stdlib.h"

#define SENSOR_PIN 25 // GPIO pin number

void infrared_sensor_init(void) {
    gpio_init(SENSOR_PIN);
    gpio_set_dir(SENSOR_PIN, GPIO_IN);
}

int infrared_sensor_read(void) {
    return gpio_get(SENSOR_PIN);
}