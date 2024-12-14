/*
 * File:   timeDelay.h
 * Authors: Ahron Ramos, Adrian Co, Zaira Ramji
 * Created on November 13, 2024, 4:28 PM
 * 
 * Description: Header file for timer delay functions. Provides initialization 
 *              and control functions for Timer1 and Timer2, allowing precise 
 *              delay operations in the embedded system.
 */

#ifndef TIMEDELAY_H
#define TIMEDELAY_H

#include <xc.h>
#include <p24F16KA101.h>

/**
 * @brief Initializes the timers (Timer1 and Timer2).
 * 
 * This function configures:
 * - Timer1: 16-bit mode with a 1:1 prescaler.
 * - Timer2: 16-bit mode with a 1:64 prescaler.
 * 
 * Enables interrupts for both timers.
 */
void timerInit();

/**
 * @brief Starts Timer1 with a specified period value.
 * 
 * Configures Timer1's period register and enables the timer.
 * 
 * @param pr_val The desired period value for Timer1.
 */
void startTimer1(uint16_t pr_val);

/**
 * @brief Stops Timer1 and resets its counter.
 * 
 * Disables Timer1 and clears its count register to reset the timer state.
 */
void stopTimer1();

/**
 * @brief Starts Timer2 with a specific delay in milliseconds.
 * 
 * Configures Timer2's period register based on the input delay and starts the timer.
 * The delay is calculated using the system clock frequency and a prescaler of 1:64.
 * 
 * @param time_ms The desired delay in milliseconds.
 */
void startTimer2(uint16_t time_ms);

/**
 * @brief Stops Timer2 and resets its counter.
 * 
 * Disables Timer2 and clears both its count register and period register.
 */
void stopTimer2();

/**
 * @brief Delays program execution for specified milliseconds using Timer3 and Idle mode
 * 
 * Uses Timer3 with PR3 as period register. The function:
 * - Calculates PR3 value based on input milliseconds
 * - Uses a prescaler of 8
 * - Enters Idle mode to save power during delay
 * - Exits when Timer3 interrupt occurs
 * 
 * @param time_ms The delay duration in milliseconds (16-bit value)
 */
void delay_ms(uint16_t time_ms);

#endif
