/*
 * File:   IOs.h
 * Authors: Ahron Ramos, Adrian Co, Zaira Ramji
 * Created on November 13, 2024, 4:28 PM
 * 
 * Description: Header file for I/O configurations and button state management.
 */

#ifndef IOS_H
#define IOS_H

#include <xc.h>
#include <p24F16KA101.h>

#define PB1 PORTAbits.RA2   // Button 1 on pin RA2
#define PB2 PORTBbits.RB4   // Button 2 on pin RB4
#define PB3 PORTAbits.RA4   // Button 3 on pin RA4

/**
 * @brief Structure to track button state and transitions
 * 
 * Each button's state is tracked using three flags:
 * @field pressed:    Set to 1 when button is released (rising edge detected)
 * @field newState:   Current state of the button (1 = released, 0 = pressed)
 * @field prevState:  Previous state of the button for edge detection
 */
typedef struct {
    uint8_t pressed;    // Button press event flag
    uint8_t newState;   // Current button state
    uint8_t prevState;  // Previous button state
} ButtonState;

// Array to track states of all three buttons
// Initialized in IOs.c with default values
extern ButtonState buttons[3];

/**
 * @brief Initializes input/output configurations for buttons and LED.
 *
 * This function sets the appropriate TRIS registers for buttons (input) 
 * and the LED (output), and enables pull-up resistors and change notifications 
 * for the buttons.
 */
void IOinit();

/**
 * @brief Checks and updates button states.
 *
 * For each button:
 * 1. Reads current button state
 * 2. Detects changes from previous state
 * 3. Sets pressed flag on rising edge (button release)
 * 4. Updates previous state for next check
 *
 * @return 0 when state check is complete
 */
uint8_t IOcheck();


#endif