/*
 * File:   stateMachine.h
 * Authors: Ahron Ramos, Adrian Co, Zaira Ramji
 * Created on November 13, 2024, 4:28 PM
 * 
 * Description: Header file for system state management and state transitions.
 */

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <xc.h>
#include <p24F16KA101.h>

/**
 * @brief Enumeration of all possible system states
 * 
 * The system can be in one of six states:
 * @field OFF_MODE:             System is in off state
 * @field OFF_BLINK:            System is off with blinking indicator
 * @field ON_MODE:              System is in on state
 * @field ON_BLINK:             System is on with blinking indicator
 * @field TRANSMIT_UART_ON:     UART transmission in on state
 * @field TRANSMIT_UART_BLINK   UART transmission with blinking indicator
 */
typedef enum {
    OFF_MODE,
    OFF_BLINK,
    ON_MODE,
    ON_BLINK,
    TRANSMIT_UART_ON,
    TRANSMIT_UART_BLINK
} state_t;

/**
 * @brief Structure to maintain system state
 * 
 * Tracks the current state of the system using the state_t enumeration
 * @field currentState: Current state of the system
 */
typedef struct {
    state_t currentState;
} SystemState;

// Global system state instance
// Initialized to OFF_MODE by default
SystemState systemState = {
    .currentState = OFF_MODE
};

#endif