/*
 * File:   IOs.c
 * Authors: Ahron Ramos, Adrian Co, Zaira Ramji
 * Created on November 13, 2024, 4:28 PM
 * 
 * Description: Implementation of I/O handling functions.
 *             Manages button state tracking.
 */

#include "IOs.h"

ButtonState buttons[3] = {
    {
        0,  // pressed:     No new press detected
        1,  // newState:    PB1 is currently released
        1   // prevState:   PB1 was previously released
    },  // PB1
    {
        0,  // pressed:     No new press detected
        1,  // newState:    PB2 is currently released
        1   // prevState:   PB2 was previously released
    },  // PB2
    {
        0,  // pressed:     No new press detected
        1,  // newState:    PB3 is currently released
        1   // prevState:   PB3 was previously released
    }   // PB3
};

void IOinit() {
    // ---- LED Configuration (RB8) ----
    TRISBbits.TRISB8 = 0;     // Set RB8 as output for LED
    LATBbits.LATB8 = 0;       // Initialize LED to OFF state
    
    // ---- Button 1 Configuration (RA2) ----
    TRISAbits.TRISA4 = 1;     // Set RA4 as input
    CNPU1bits.CN0PUE = 1;     // Enable internal pull-up resistor
    CNEN1bits.CN0IE = 1;      // Enable change notification interrupt
    
    // ---- Button 2 Configuration (RB4) ----
    TRISBbits.TRISB4 = 1;     // Set RB4 as input
    CNPU1bits.CN1PUE = 1;     // Enable internal pull-up resistor
    CNEN1bits.CN1IE = 1;      // Enable change notification interrupt
    
    // ---- Button 3 Configuration (RA4) ----
    TRISAbits.TRISA2 = 1;     // Set RA2 as input
    CNPU2bits.CN30PUE = 1;    // Enable internal pull-up resistor
    CNEN2bits.CN30IE = 1;     // Enable change notification interrupt
    
    // ---- Change Notification Interrupt Configuration ----
    IPC4bits.CNIP = 6;        // Set interrupt priority
    IFS1bits.CNIF = 0;        // Clear interrupt flag
    IEC1bits.CNIE = 1;        // Enable change notification interrupts
}

uint8_t IOcheck() {
    // Check each button's state
    for (int i = 0; i < 3; i++) {
        // Get current state of appropriate button
        buttons[i].newState = (i == 0) 
                ? PB1 
                : (i == 1) 
                    ? PB2 
                    : PB3;
        
        // Check for state change
        if (buttons[i].newState != buttons[i].prevState) {
            // Detect rising edge (button release)
            if (buttons[i].prevState == 0 && buttons[i].newState == 1) {
                buttons[i].pressed = 1;  // Set pressed flag
            }
            // Update previous state
            buttons[i].prevState = buttons[i].newState;
        }
    }
    
    return 0;
}