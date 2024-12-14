/*
 * File:   ADC.c
 * Authors: Ahron Ramos, Adrian Co, Zaira Ramji
 * Created on November 13, 2024, 4:28 PM
 * 
 * Description: Implementation of ADC functions.
 *             Handles configuration and operation of 10-bit ADC for analog input AN5.
 */

#include "ADC.h"

void init_ADC() {
    // ---- AD1CON1 Register Configuration ----
    AD1CON1bits.ADSIDL = 0;         // Continue ADC operation in idle mode
    AD1CON1bits.FORM = 0b00;        // Output in integer format
    AD1CON1bits.SSRC = 0b111;       // Internal counter ends sampling and starts conversion
    AD1CON1bits.ASAM = 0;           // Manual sampling start
    
    // ---- AD1CON2 Register Configuration ----
    AD1CON2bits.VCFG = 0b000;       // Voltage Reference Configuration
                                    // AVdd as positive reference
                                    // AVss as negative reference
    
    AD1CON2bits.CSCNA = 0;          // Do not scan inputs 
    AD1CON2bits.SMPI = 0b0000;      // Generate interrupt after each conversion
    AD1CON2bits.BUFM = 0;           // Buffer configured as one 16-word buffer
    AD1CON2bits.ALTS = 0;           // Always use input multiplexer A
    
    // ---- AD1CON3 Register Configuration ----
    AD1CON3bits.ADRC = 0;           // Use system clock for ADC conversion
    AD1CON3bits.SAMC = 0b11111;     // Auto-sample time = 31 TAD
                                    // Longer sample time = more accurate reading

    AD1CON3bits.ADCS = 0b111111;    // ADC Conversion Clock = 64 × TCY
                                    // Slower clock = more accurate reading
    
    // ---- Channel Selection Configuration ----
    AD1CHSbits.CH0NA = 0;           // Negative input is AVss
    AD1CHSbits.CH0SA = 0b00101;     // Positive input is AN5 (Channel 5)
    
    // ---- Port Configuration ----
    TRISAbits.TRISA3 = 1;           // Set AN5 pin as input
    AD1PCFGbits.PCFG5 = 0;          // Configure AN5 as analog input
    AD1CSSLbits.CSSL5 = 0;          // Remove AN5 from input scan
}

uint16_t do_ADC() {
    uint16_t ADCvalue;
    
    // Start sampling
    AD1CON1bits.ADON = 1;           // Turn on ADC module
    AD1CON1bits.SAMP = 1;           // Start sampling
    
    // Wait for conversion to complete
    while(!AD1CON1bits.DONE);       // DONE bit set when conversion finishes
    
    // Get result
    ADCvalue = ADC1BUF0;            // Read the ADC conversion result
    AD1CON1bits.SAMP = 0;           // Stop sampling
    AD1CON1bits.ADON = 0;           // Turn off ADC module to save power
    
    return ADCvalue;                // Return 10-bit result (0-1023)
}