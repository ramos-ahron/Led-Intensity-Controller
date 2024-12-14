/*
 * File:   PWM.h
 * Authors: Ahron Ramos, Adrian Co, Zaira Ramji
 * Created on November 13, 2024, 4:28 PM
 * 
 * Description: Header file for PWM-based LED control.
 *             Manages LED brightness and blinking based on ADC input or manual control.
 */

#ifndef PWM_H
#define PWM_H

#include <xc.h>
#include <p24F16KA101.h>
#include "ADC.h"
#include "UART2.h"
#include "timeDelay.h"

/**
 * @brief Structure to manage all PWM-related parameters and states
 * 
 * Centralizes PWM control variables for LED brightness and blinking:
 * @field period:           PWM period value (determines frequency)
 * @field baseDutyCycle:    Base brightness level (0-period)
 * @field blinkDutyCycle:   Brightness level during blink ON state
 * @field currentDutyCycle: Currently active duty cycle
 * @field blinkEnabled:     Flag to indicate if blinking is active
 * @field blinkState:       Current state of blink (ON/OFF)
 * @field adcValue:         Latest ADC reading (0-1023)
 */
typedef struct {
    uint8_t period;           // PWM period
    uint8_t baseDutyCycle;    // Normal brightness level
    uint8_t blinkDutyCycle;   // Blink ON state brightness
    uint8_t currentDutyCycle; // Active duty cycle value
    uint8_t blinkEnabled;     // Blinking mode flag
    uint8_t blinkState;       // Current blink state
    uint16_t adcValue;        // Latest ADC reading
} PWMControl;

// Global PWM control structure
// Initialized in PWM.c with default values
extern PWMControl pwmControl;

/**
 * @brief Updates LED brightness based on ADC or manual control
 *
 * Operation modes:
 * 1. ADC-controlled (overrideDutyCycle = 0):
 *    - Reads ADC value
 *    - Scales to PWM range
 *    - Updates duty cycle
 * 
 * 2. Manual control (overrideDutyCycle > 0):
 *    - Uses provided duty cycle directly
 *
 * In both modes, final brightness considers blinking state
 *
 * @param overrideDutyCycle Manual duty cycle value (0 for ADC control)
 */
void updateBrightness(uint8_t overrideDutyCycle);

/**
 * @brief Enables LED blinking mode
 *
 * Sets up blinking:
 * 1. Enables blink flag
 * 2. Starts Timer2 with 500ms period
 * 3. Uses current base duty cycle for ON state
 */
void blink();

/**
 * @brief Disables LED blinking mode
 *
 * Cleanup actions:
 * 1. Disables blink flag
 * 2. Stops Timer2
 * 3. Restores continuous brightness
 */
void stopBlink();

/**
 * @brief Sends PWM and ADC data via UART
 *
 * Transmits:
 * 1. Duty cycle as percentage (0-100)
 * 2. Space character separator
 * 3. Raw ADC value (0-1023)
 * 4. Newline character
 */
void transmitVoltageADC();

#endif
