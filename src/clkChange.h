/*
 * File:   clkChange.h
 * Authors: Ahron Ramos, Adrian Co, Zaira Ramji
 * Created on November 13, 2024, 4:28 PM
 */

#ifndef CLKCHANGE_H
#define CLKCHANGE_H

#include <xc.h>
#include <p24F16KA101.h>

/**
 * @brief Sets the microcontroller's clock frequency.
 *
 * @param clkval Desired clock value (in Hz).
 */
void newClk(unsigned int clkval);

#endif // CLKCHANGE_H
