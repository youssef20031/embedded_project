// led_lamp.h

#ifndef LED_LAMP_H
#define LED_LAMP_H

#include <stdint.h>

class LEDLamp {
public:
    LEDLamp(uint32_t pin);
    void turnOn();
    void turnOff();

private:
    uint32_t gpio_pin;
};

#endif // LED_LAMP_H