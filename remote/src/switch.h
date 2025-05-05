#ifndef SWITCH_H
#define SWITCH_H

void switchInit();
uint8_t switchRead(uint8_t pin);
void switchWrite(uint8_t pin, uint8_t value); // Only for test toggling or LEDs, etc.

#endif
