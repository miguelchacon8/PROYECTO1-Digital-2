/* 
 * File:   
 * Author: Luis Carranza
 * Comments:
 * Revision history: 24/10/22
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef setupADC_H
#define	setupADC_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

// TODO Insert appropriate #include <>

// TODO Insert declarations

void ADC_config(int channel);
uint16_t ADC_read(int channel);


#endif	/* XC_HEADER_TEMPLATE_H */
