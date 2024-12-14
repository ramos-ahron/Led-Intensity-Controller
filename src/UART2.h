/*
 * File:   UART2.h
 * Author: Ahron Ramos, Adrian Co, Zaira Ramji
 * Created on November 13, 2024, 4:28 PM
 *
 * Description: Header file for UART2 serial communication module.
 *             Provides functions for character, string, and numeric data transmission
 *             with support for various display formats (hex, decimal).
 */

#ifndef UART2_H
#define UART2_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @brief Initializes UART2 module for serial communication
     *
     * Configuration details:
     * 1. Pin assignments:
     *    - RB0: TX (transmit)
     *    - RB1: RX (receive)
     *
     * 2. Communication parameters:
     *    - Data: 8-bit
     *    - Parity: None
     *    - Stop bits: 1
     *    - Mode: High-speed (16x clock)
     *
     * 3. Baud rates (auto-selected):
     *    - 8MHz clock:   9600 baud
     *    - 500kHz clock: 4800 baud
     *    - 32kHz clock:  300 baud
     *
     * 4. Interrupts:
     *    - TX enabled (priority 3)
     */
    void InitUART2(void);

    /**
     * @brief Transmits a single character multiple times
     *
     * Sends the specified character repeatedly over UART2.
     * Blocks until all transmissions are complete.
     *
     * @param character     ASCII character to transmit
     * @param repeatCount   Number of times to transmit the character
     */
    void XmitUART2(char character, unsigned int repeatCount);

    /**
     * @brief Displays a 12-bit value in hexadecimal format
     *
     * Output format: " 0xXXX " (space-padded)
     * Example: " 0xFFF "
     *
     * @param value         Number to display (0-4095)
     */
    void Disp2Hex(unsigned int value);

    /**
     * @brief Displays a 32-bit value in hexadecimal format
     *
     * Output format: " 0xXXXXXXXX " (space-padded)
     * Example: " 0x0000FFFF "
     *
     * @param value         Number to display (0-4294967295)
     */
    void Disp2Hex32(unsigned long int value);

    /**
     * @brief Displays a null-terminated string
     *
     * Transmits each character of the string sequentially.
     * Stops at null terminator.
     *
     * @param str           Pointer to null-terminated string
     * @warning             str must be properly null-terminated
     */
    void Disp2String(const char *str);

    /**
     * @brief Displays a 16-bit value in decimal format
     *
     * Output format: " XXXXX " (zero-padded, space-padded)
     * Example: " 00042 "
     *
     * @param value         Number to display (0-65535)
     */
    void Disp2Dec(uint16_t value);

    /**
     * @brief Displays a number with specified digit count
     *
     * Output format: XXXXX (zero-padded to specified digits)
     * Example: "00042" (digits = 5)
     *
     * @param number        Value to display
     * @param digits        Number of digits to display
     */
    void DispNum(uint16_t number, uint8_t digits);

    /**
     * @brief TX interrupt handler for UART2
     *
     * Called automatically when TX operations complete.
     * Manages transmission buffer and status flags.
     */
    void __attribute__((interrupt, no_auto_psv)) _U2TXInterrupt(void);

#ifdef __cplusplus
}
#endif

#endif