/*
 * File:   PWM.c
 * Authors: Ahron Ramos, Adrian Co, Zaira Ramji
 * Created on November 13, 2024, 4:28 PM
 *
 * Description: Implementation of PWM-based LED control functions.
 *             Handles brightness adjustment and blinking patterns.
 */

#include "PWM.h"

// Initialize PWM control structure with default values
PWMControl pwmControl = {
    50, // period:              PWM frequency
    0,  // baseDutyCycle:       start with LED off
    0,  // blinkDutyCycle:      initial blink brightness
    0,  // currentDutyCycle:    start with LED off
    0,  // blinkEnabled:        start in steady mode
    0   // blinkState:          start in OFF state
};

void updateBrightness(uint8_t overrideDutyCycle)
{
    // Start Timer1 for PWM timing
    startTimer1(pwmControl.period);

    // Get new ADC reading
    pwmControl.adcValue = do_ADC();

    // Calculate base duty cycle
    if (!overrideDutyCycle)
    {
        // Scale ADC value (0-1023) to duty cycle range (0-period)
        pwmControl.baseDutyCycle = (uint8_t)((uint32_t)pwmControl.adcValue *
                                             pwmControl.period / 1023);
    }
    else
    {
        // Use provided duty cycle directly
        pwmControl.baseDutyCycle = overrideDutyCycle;
    }

    // Set current duty cycle based on mode
    if (!pwmControl.blinkEnabled)
    {
        // Normal mode: use base duty cycle
        pwmControl.currentDutyCycle = pwmControl.baseDutyCycle;
    }
    else
    {
        pwmControl.blinkDutyCycle = pwmControl.baseDutyCycle;

        // Blinking mode: alternate between duty cycle and off
        pwmControl.currentDutyCycle = (pwmControl.blinkState) ?
                                // ON state: use base duty cycle
                                pwmControl.blinkDutyCycle :
                                // OFF state: save duty cycle but output 0
                                0;
    }
}

void blink()
{
    pwmControl.blinkEnabled = 1; // Enable blinking mode
    startTimer2(500);            // Start blink timer
}

void stopBlink()
{
    pwmControl.blinkEnabled = 0; // Disable blinking mode
    stopTimer2();                // Stop blink timer
}

void transmitVoltageADC()
{
    // Convert duty cycle to percentage (0-100)
    uint8_t dutyPercent = (uint32_t)pwmControl.currentDutyCycle * 100 /
                          pwmControl.period;

    // Transmit duty cycle percentage
    DispNum(dutyPercent, 3);
    XmitUART2(' ', 1);

    // Transmit ADC value
    DispNum(pwmControl.adcValue, 4);
    XmitUART2('\n', 1);
}