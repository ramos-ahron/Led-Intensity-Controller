/*
 * File:   main.c
 * Authors: Ahron Ramos, Adrian Co, Zaira Ramji
 * Created on November 13, 2024, 4:28 PM
 */

// Configuration settings for the PIC microcontroller

// FBS
#pragma config BWRP = OFF               // Boot segment may be written
#pragma config BSS = OFF                // No boot program Flash segment

// FGS
#pragma config GWRP = OFF               // General segment may be written
#pragma config GCP = OFF                // No protection on general segment code

// FOSCSEL
#pragma config FNOSC = FRC              // Fast RC oscillator selected
#pragma config IESO = OFF               // Internal/external switch over disabled

// FOSC
#pragma config POSCMOD = NONE           // Primary oscillator disabled
#pragma config OSCIOFNC = ON            // CLKO output disabled; pin functions as port I/O
#pragma config POSCFREQ = HS            // External clock frequency greater than 8 MHz
#pragma config SOSCSEL = SOSCHP         // Secondary oscillator configured for high-power operation
#pragma config FCKSM = CSECMD           // Clock switching enabled, Fail-Safe Clock Monitor disabled

// FWDT
#pragma config WDTPS = PS32768          // Watchdog Timer Postscale (1:32,768)
#pragma config FWPSA = PR128            // WDT prescaler ratio of 1:128
#pragma config WINDIS = OFF             // Standard WDT selected; windowed WDT disabled
#pragma config FWDTEN = OFF             // WDT disabled

// FPOR
#pragma config BOREN = BOR3             // Brown-out Reset enabled in hardware
#pragma config PWRTEN = ON              // Power-up Timer enabled
#pragma config I2C1SEL = PRI            // Default location for I2C1 pins
#pragma config BORV = V18               // Brown-out Reset set to lowest voltage (1.8V)
#pragma config MCLRE = ON               // MCLR pin enabled

// FICD
#pragma config ICS = PGx2               // PGC2/PGD2 used for programming/debugging

// FDS
#pragma config DSWDTPS = DSWDTPSF       // Deep Sleep Watchdog Timer Postscale (1:2,147,483,648)
#pragma config DSWDTOSC = LPRC          // DSWDT uses LPRC as reference clock
#pragma config RTCOSC = SOSC            // RTCC uses SOSC as reference clock
#pragma config DSBOREN = ON             // Deep Sleep BOR enabled
#pragma config DSWDTEN = ON             // DSWDT enabled

// Include necessary headers
#include <xc.h>
#include <p24F16KA101.h>
#include "stateMachine.h"
#include "clkChange.h"
#include "timeDelay.h"
#include "UART2.h"
#include "PWM.h"
#include "ADC.h"
#include "IOs.h"

/**
 * Pin Definitions:
 * - PB1: Push button 1, used for toggling system modes (OFF/ON).
 * - PB2: Push button 2, used for enabling/disabling blinking modes.
 * - PB3: Push button 3, used for UART transmission trigger.
 * - LED: Output pin controlling the LED (brightness or blinking).
 */
#define PB1 PORTAbits.RA2               // Button 1 on pin RA2
#define PB2 PORTBbits.RB4               // Button 2 on pin RB4
#define PB3 PORTAbits.RA4               // Button 3 on pin RA4
#define LED LATBbits.LATB8              // LED on pin RB8

/**
 * Global Variables:
 * - pwmCounter: Tracks cycles for PWM timing.
 * - buttonFlag: Flag to indicate button press events.
 */
uint8_t pwmCounter = 0;
uint8_t buttonFlag = 0;

/**
 * @brief Initializes system configuration and peripherals.
 *
 * Performs the following setup tasks:
 * - Configures all pins as digital I/O
 * - Sets the clock frequency
 * - Initializes timers
 * - Configures I/O pins
 * - Sets up UART communication for terminal display
 * - Configures the ADC module for analog-to-digital conversion
 * - Clears the terminal screen for a fresh user interface
 *
 * This function ensures the embedded system is ready for operation
 * by preparing all necessary hardware and communication interfaces.
 */
void init();

/**
 * @brief Handles transitions between system states based on button inputs.
 *
 * Transitions system states and manages associated actions such as:
 * - Enabling/disabling LED blinking
 * - Adjusting LED brightness via PWM
 * - Sending ADC data via UART
 * 
 * The function performs the following:
 * - Reads button inputs to determine state changes
 * - Executes corresponding actions for each state
 */
void handleStateTransition();

/**
 * @brief Main program entry point for system operation.
 *
 * Initializes system settings and enters an infinite loop to check button states,
 * handle state transitions, and reset button flags.
 */
int main() {
    init();
    while (1) {   
        if (buttonFlag) {
            buttonFlag = IOcheck();
        }

        handleStateTransition();

        // Clear button press flags after processing state transitions
        for (int i = 0; i < 3; i++) {
            buttons[i].pressed = 0;
        }
    }
    
    return 0;
}

void init() {
    AD1PCFG = 0xFFFF;               // Configure all pins as digital
    newClk(500);                    // Set the clock frequency
    timerInit();                    // Initialize timer
    IOinit();                       // Initialize I/O pins
    InitUART2();                    // Initialize UART communication    
    init_ADC();                     // Initialize ADC
    Disp2String("\033[2J\033[H");   // Clear the terminal screen
}

void handleStateTransition() {
    switch (systemState.currentState) {
        case OFF_MODE:
            // System completely off - LED disabled and timers stopped
            delay_ms(20);
            LED = 0;
            stopBlink();        // Disable blinking mode
            stopTimer1();       // Stop PWM timer
            stopTimer2();       // Stop blink timer

            // State transitions:
            // PB1 -> ON_MODE: Turn system on
            // PB2 -> OFF_BLINK: Enter blinking mode
            if (buttons[0].pressed) systemState.currentState = ON_MODE;
            else if (buttons[1].pressed) systemState.currentState = OFF_BLINK;
            break;
                
        case OFF_BLINK:
            // System off but LED blinking at max brightness
            blink();                             // Enable blink functionality
            updateBrightness(pwmControl.period); // Set to max brightness

            // State transitions:
            // PB2 -> OFF_MODE: Return to fully off state
            if (buttons[1].pressed) {
                systemState.currentState = OFF_MODE;
                stopBlink();
            }
            break;

        case ON_MODE:
            // System on with LED at brightness determined by ADC
            updateBrightness(0); // Set LED brightness from ADC

            // State transitions:
            // PB1 -> OFF_MODE: Turn system off
            // PB2 -> ON_BLINK: Enter blinking while on
            // PB3 -> TRANSMIT_UART_ON: Start UART transmission while on
            if (buttons[0].pressed) systemState.currentState = OFF_MODE;
            else if (buttons[1].pressed) systemState.currentState = ON_BLINK;
            else if (buttons[2].pressed) systemState.currentState = TRANSMIT_UART_ON;
            break;

        case ON_BLINK:
            // System on with LED blinking
            blink();                // Enable blinking mode
            updateBrightness(0);    // Set LED brightness from ADC

            // State transitions:
            // PB2 -> ON_MODE: Return to steady LED
            // PB3 -> TRANSMIT_UART_BLINK: Start UART transmission while blinking
            if (buttons[1].pressed) {
                systemState.currentState = ON_MODE;
                stopBlink();
            } else if (buttons[2].pressed) systemState.currentState = TRANSMIT_UART_BLINK;
            break;

        case TRANSMIT_UART_ON:
            // System on with LED steady and UART transmission active
            updateBrightness(0);     // Set LED brightness from ADC
            transmitVoltageADC();    // Transmit ADC voltage over UART

            // State transitions:
            // PB1 -> OFF_MODE: Turn system off
            // PB2 -> TRANSMIT_UART_BLINK: Switch to blinking with UART
            // PB3 -> ON_MODE: Stop UART transmission, remain on
            if (buttons[0].pressed) systemState.currentState = OFF_MODE;
            else if (buttons[1].pressed) systemState.currentState = TRANSMIT_UART_BLINK;
            else if (buttons[2].pressed) systemState.currentState = ON_MODE;
            break;

        case TRANSMIT_UART_BLINK:
            // System on with LED blinking and UART transmission active
            blink();                // Enable blinking mode
            updateBrightness(0);    // Set LED brightness from ADC
            transmitVoltageADC();   // Transmit ADC voltage over UART

            // State transitions:
            // PB2 -> TRANSMIT_UART_ON: Switch to steady on with UART
            // PB3 -> ON_BLINK: Stop UART transmission, continue blinking
            if (buttons[1].pressed) {
                systemState.currentState = TRANSMIT_UART_ON;
                stopBlink();
            } else if (buttons[2].pressed) systemState.currentState = ON_BLINK;
            break;

        default:
            // Handle undefined states by resetting to OFF_MODE
            systemState.currentState = OFF_MODE;
            break;
    }
}

/**
 * @brief Timer 1 interrupt service routine for PWM generation.
 *
 * Implements software PWM by:
 * 1. Incrementing counter within PWM period
 * 2. Setting current duty cycle based on blink state
 * 3. Controlling LED based on counter vs duty cycle comparison
 * 
 * PWM Operation:
 * - Counter cycles from 0 to period-1
 * - LED turns on when counter < duty cycle
 * - Duty cycle varies based on blink state and base brightness
 */
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) {
    // Increment and wrap PWM counter within period
    pwmCounter = (pwmCounter + 1) % pwmControl.period;
    
    // If blinking enabled, use current duty cycle, else use base duty cycle
    pwmControl.currentDutyCycle = pwmControl.blinkEnabled 
            ? pwmControl.currentDutyCycle 
            : pwmControl.baseDutyCycle;
    
    // LED on when counter less than duty cycle
    LED = (pwmCounter < pwmControl.currentDutyCycle) ? 1 : 0;
    
    IFS0bits.T1IF = 0;  // Clear Timer1 interrupt flag
}

/**
 * @brief Timer 2 interrupt service routine for blink timing.
 *    
 * Controls LED blinking by:
 * 1. Toggling blink state at regular intervals
 * 2. Setting duty cycle to either blink level or 0 based on state
 * 
 * Blinking is implemented using PWM for smooth transitions:
 * - When blinkState true: LED at specified blink brightness
 * - When blinkState false: LED off (duty cycle = 0)
 */
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void) {
    if (pwmControl.blinkEnabled) {
        // Toggle between on and off states
        pwmControl.blinkState = !pwmControl.blinkState;
        
        // Set duty cycle based on blink state
        pwmControl.currentDutyCycle = pwmControl.blinkState 
                ? pwmControl.blinkDutyCycle  // During 'on' phase
                : 0;                         // During 'off' phase
    }
    IFS0bits.T2IF = 0;  // Clear Timer2 interrupt flag
}

/**
 * @brief Timer 3 interrupt service routine
 *
 */
void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void) {
    IFS0bits.T3IF = 0;  // Clear Timer 3 interrupt flag
}

/**
 * @brief Change Notification interrupt service routine.
 *
 */
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void) {
    buttonFlag = 1;      // Signal main loop to process button states
    IFS1bits.CNIF = 0;   // Clear CN interrupt flag
}
