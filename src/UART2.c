/*
 * File:   UART2.c
 * Author: Ahron Ramos, Adrian Co, Zaira Ramji
 * Created on November 13, 2024, 4:28 PM
 *
 * Description: Implementation file for UART2 serial communication module.
 *              This module provides functions for initializing and using
 *              UART2 for data transmission. It includes character and string
 *              transmission, as well as functions to display numeric data
 *              in various formats (hexadecimal, decimal).
 *              The implementation is designed for high-speed,
 *              interrupt-driven operation and supports configurable baud rates.
 */

#include "xc.h"
#include "math.h"
#include "string.h"
#include "UART2.h"

// Static lookup table for hex conversion
static const char HEX_CHARS[] = "0123456789ABCDEF";

void InitUART2(void) {
    // Configure I/O pins
    TRISBbits.TRISB0 = 0;           // TX output
    TRISBbits.TRISB1 = 1;           // RX input
    LATBbits.LATB0 = 1;             // TX idle high

    // Configure UART2: 8-bit, no parity, 1 stop bit, high-speed mode
    U2MODE = 0x0008;                // BRGH = 1 (16x clock)

    // Set baud rate based on system clock frequency
    static const uint16_t BAUD_RATES[] = {103, 12, 12}; // 8MHz, 500kHz, 32kHz
    U2BRG = BAUD_RATES[OSCCONbits.COSC & 0x7];

    // Configure TX interrupts and enable UART
    U2STA = 0xA000;                 // Interrupt on TX complete
    IFS1bits.U2TXIF = 0;            // Clear TX flag
    IPC7bits.U2TXIP = 3;            // TX priority
    IEC1bits.U2TXIE = 1;            // Enable TX interrupt

    U2MODEbits.UARTEN = 1;          // Enable UART
    U2STAbits.UTXEN = 1;            // Enable transmitter
}

inline void WaitForTXReady(void) {
    while (U2STAbits.UTXBF)
        ; // Wait for TX buffer
}

void XmitUART2(char character, unsigned int count) {
    while (count--) {
        WaitForTXReady();
        U2TXREG = character;
    }
    while (!U2STAbits.TRMT)
        ; // Wait for completion
}

void Disp2Hex(unsigned int value) {
    char output[6] = {' ', '0', 'x'};

    // Convert 3 nibbles (12 bits)
    for (int i = 0; i < 3; i++) {
        output[5 - i] = HEX_CHARS[value & 0xF];
        value >>= 4;
    }
    output[5] = ' ';

    // Send all characters at once
    for (int i = 0; i < 6; i++) {
        WaitForTXReady();
        U2TXREG = output[i];
    }
}

void Disp2Hex32(unsigned long value) {
    char output[11] = {' ', '0', 'x'};

    // Convert 8 nibbles (32 bits)
    for (int i = 0; i < 8; i++) {
        output[10 - i] = HEX_CHARS[value & 0xF];
        value >>= 4;
    }
    output[10] = ' ';

    // Send all characters at once
    for (int i = 0; i < 11; i++) {
        WaitForTXReady();
        U2TXREG = output[i];
    }
}

void Disp2String(const char *str) {
    while (*str) {
        WaitForTXReady();
        U2TXREG = *str++;
    }
}

void Disp2Dec(uint16_t value) {
    char output[7] = {' '};
    uint16_t divisor = 10000;       // 5 digits max

    // Convert to ASCII digits
    for (int i = 1; i < 6; i++) {
        output[i] = '0' + (value / divisor);
        value %= divisor;
        divisor /= 10;
    }
    output[6] = ' ';

    // Send all characters at once
    for (int i = 0; i < 7; i++) {
        WaitForTXReady();
        U2TXREG = output[i];
    }
}

void DispNum(uint16_t number, uint8_t digits) {
    uint16_t divisor = 1;

    // Calculate initial divisor
    for (uint8_t i = 1; i < digits; i++) {
        divisor *= 10;
    }

    // Output digits
    do {
        WaitForTXReady();
        U2TXREG = '0' + ((number / divisor) % 10);
        divisor /= 10;
    } while (divisor);
}

// Interrupt handlers
void __attribute__((interrupt, no_auto_psv)) _U2TXInterrupt(void) {
    IFS1bits.U2TXIF = 0;
}