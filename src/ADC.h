/*
 * File:   ADC.h
 * Authors: Ahron Ramos, Adrian Co, Zaira Ramji
 * Created on November 13, 2024, 4:28 PM
 * 
 * Description: Header file for ADC configuration and operation on microcontroller.
 *             Provides functions for initializing and reading from analog input AN5.
 */

#ifndef ADC_H
#define ADC_H

#include <xc.h>
#include <p24F16KA101.h>

/**
 * @brief Initializes the Analog-to-Digital Converter module.
 *
 * Configures ADC settings, including reference voltages, sampling, and channel selection, 
 * to prepare for analog input readings on AN5.
 */
void init_ADC();

/**
 * @brief Performs an ADC conversion on the selected analog input.
 *
 * Starts sampling, waits for the conversion to complete, and returns the resulting 16-bit value.
 * 
 * @return uint16_t The converted analog value from AN5.
 */
uint16_t do_ADC();

#endif