/*
 * File:   oscilador.c
 * Author: Luis Pablo Carranza
 *
 * Created on 24 de octubre de 2022, 11:37 PM
 */

#include "setupADC.h"
#define _XTAL_FREQ 2000000

void ADC_config(int channel){
    if((channel & 0x01) == 1){
        PORTAbits.RA0 = 0;      // Inicia el bit 0 de PORTA en 0
        TRISAbits.TRISA0 = 1;   // RA0 es entrada
        ANSELbits.ANS0 = 1;     // RA0 es analógico    
    }
    if((channel & 0x02) == 0x02){
        PORTAbits.RA1 = 0;      // Inicia el bit 1 de PORTA en 0
        TRISAbits.TRISA1 = 1;   // RA1 es entrada
        ANSELbits.ANS1 = 1;     // RA1 es analógico  
    
    }
    if((channel & 0x04) == 0x4){
        PORTAbits.RA2 = 0;      // Inicia el bit 2 de PORTA en 0
        TRISAbits.TRISA2 = 1;   // RA2 es entrada
        ANSELbits.ANS2 = 1;     // RA2 es analógico  
    }
    
    ADCON0bits.ADCS1 = 0;
    ADCON0bits.ADCS0 = 1;   // Fosc/8
    
    ADCON1bits.VCFG1 = 0;   // Ref VSS
    ADCON1bits.VCFG0 = 0;   // Ref VDD
    
    ADCON1bits.ADFM = 0;    // Justificado a la izquierda
    
    ADCON0bits.ADON = 1;    // Habilitar el convertidor ADC
    __delay_us(100);
}

uint16_t ADC_read(int channel){
    if(channel ==0){
        ADCON0bits.CHS = 0b0000;    // Canal analógico 0
        __delay_us(100);
        ADCON0bits.GO = 1;          // Iniciar la conversión ADC
        while (ADCON0bits.GO);
        ADIF = 0;
        return ADRESH;
    }
    if(channel ==1){
        ADCON0bits.CHS = 0b0001;    // Canal analógico 1
        __delay_us(100);
        ADCON0bits.GO = 1;          // Iniciar la conversión ADC
        while (ADCON0bits.GO);
        ADIF = 0;
        return ADRESH;
    }
    if(channel ==2){
        ADCON0bits.CHS = 0b0010;    // Canal analógico 2
        __delay_us(100);
        ADCON0bits.GO = 1;          // Iniciar la conversión ADC
        while (ADCON0bits.GO);
        ADIF = 0;
        return ADRESH;
    }
}