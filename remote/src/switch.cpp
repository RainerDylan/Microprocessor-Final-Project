#include <avr/io.h>
#include "switch.h"

// Initializes PA0, PA1, PA2, PA3, PA6 as INPUT with internal pull-ups
void switchInit() {
    DDRA &= ~((1 << DDA0) | (1 << DDA1) | (1 << DDA2) | (1 << DDA3) | (1 << DDA6)); // Inputs
    PORTA |= (1 << PA0) | (1 << PA1) | (1 << PA2) | (1 << PA3) | (1 << PA6);        // Pull-ups
}

// Reads state of a given PAx pin (returns 1 if HIGH, 0 if LOW)
uint8_t switchRead(uint8_t pin) {
    return (PINA & (1 << pin)) ? 1 : 0;
}

// Optional: write to PORTA pin (e.g. for test LEDs or toggling outputs)
void switchWrite(uint8_t pin, uint8_t value) {
    DDRA |= (1 << pin); // Set pin as output
    if (value)
        PORTA |= (1 << pin);
    else
        PORTA &= ~(1 << pin);
}
