#include "adc.h"

void initADC()
{
    ADMUX |= (1 << REFS0);     // Set reference voltage to AVcc (5V)
    ADMUX &= ~(1 << REFS1);    // Ensure REFS1 is cleared for AVcc ref
    ADMUX &= ~(1 << ADLAR);    // Right-adjust the ADC result
    ADCSRB &= ~(1 << MUX5);    // Clear MUX5 for single-ended ADC input

    // Select ADC0 (pin A0) by clearing MUX[4:0]
    ADMUX &= ~((1 << MUX0) | (1 << MUX1) | (1 << MUX2));
    ADMUX &= ~((1 << MUX3) | (1 << MUX4));

    // Set ADC to Free Running Mode (continuous sampling)
    ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));

    // Enable ADC and auto trigger
    ADCSRA |= (1 << ADEN) | (1 << ADATE);

    // Set ADC prescaler to 64 for ~125kHz ADC clock (assuming 8MHz system clock)
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1);

    DIDR0 |= (1 << ADC0D);     // Disable digital input buffer on ADC0
    ADCSRA |= (1 << ADSC);     // Start the first ADC conversion
}
