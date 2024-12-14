/*
 * File:   timeDelay.c
 * Authors: Ahron Ramos, Adrian Co, Zaira Ramji
 * Created on November 13, 2024, 4:28 PM
 */

#include "timeDelay.h"

void timerInit() {
    // Timer2 Configuration
    T2CONbits.T32   = 0;                // Operate Timer2 as a 16-bit timer (not 32-bit mode)

    // Timer1 Initialization
    T1CONbits.TCKPS = 0;                // Timer1 prescaler: 1:1
    T1CONbits.TCS   = 0;                // Internal clock source
    T1CONbits.TSIDL = 0;                // Operate during CPU idle mode
    IPC5bits.INT1IP = 2;                // Timer1 interrupt priority: 2
    IFS0bits.T1IF   = 0;                // Clear Timer1 interrupt flag
    IEC0bits.T1IE   = 1;                // Enable Timer1 interrupts
    
    // Timer2 Initialization
    T2CONbits.TCKPS = 2;                // Timer2 prescaler: 1:64
    T2CONbits.TCS   = 0;                // Internal clock source
    T2CONbits.TSIDL = 0;                // Operate during CPU idle mode
    IPC1bits.T2IP   = 3;                // Timer2 interrupt priority: 3
    IFS0bits.T2IF   = 0;                // Clear Timer2 interrupt flag
    IEC0bits.T2IE   = 1;                // Enable Timer2 interrupts

    // Timer 3 Initialization
    T3CONbits.TCKPS = 1;                // Set Timer3 prescaler to 1:8
    T3CONbits.TCS   = 0;                // Use the internal clock source for Timer3
    T3CONbits.TSIDL = 0;                // Timer3 continues to operate in CPU idle mode
    IPC2bits.T3IP   = 4;                // Set Timer3 interrupt priority to 4
    IFS0bits.T3IF   = 0;                // Clear Timer3 interrupt flag
    IEC0bits.T3IE   = 1;                // Enable Timer3 interrupts
}

void startTimer1(uint16_t pr_val) {
    PR1 = pr_val;                       // Set Timer1 period
    T1CONbits.TON = 1;                  // Turn on Timer1
}

void stopTimer1() {
    TMR1 = 0;                           // Reset Timer1 counter
    T1CONbits.TON = 0;                  // Turn off Timer1
}

void startTimer2(uint16_t time_ms) {
    // Calculate PR value: (time_ms * clock frequency) / (prescaler * 2)
    PR2 = (uint16_t)(((uint32_t)time_ms) * 500 / 128);
    T2CONbits.TON = 1;                  // Turn on Timer2
}

void stopTimer2() {
    TMR2 = 0;                           // Reset Timer2 counter
    PR2 = 0;                            // Reset Timer2 period
    T2CONbits.TON = 0;                  // Turn off Timer2
}

void delay_ms(uint16_t time_ms) {
    PR3 = (uint16_t)(((uint32_t)time_ms) * 500 / 16);
    TMR3 = 0;
    T3CONbits.TON = 1;
    Idle();
    T3CONbits.TON = 0;
}