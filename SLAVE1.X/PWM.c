/*
 * File:   PWM.c
 * Author: Migue
 *
 * Created on 22 de febrero de 2023, 05:55 PM
 */


#include <xc.h>

void setupPWM(void){
    // Paso 1
    TRISCbits.TRISC2 = 1; 
    // Paso 2
    PR2 = 155;      // Periodo de 20mS  
    // Paso 3
    CCP1CON = 0b00001100;        // P1A como PWM 
   // Paso 4
    CCP1CONbits.DC1B = valDCL;        // CCPxCON<5:4>
    CCPR1L = valDCH ;        // CCPR1L   
    // Paso 5
    TMR2IF = 0;
    T2CONbits.T2CKPS = 0b11;      // Prescaler de 1:16
    TMR2ON = 1;         // Encender timer 2 
    // Paso 6
    while(!TMR2IF);
    TRISCbits.TRISC2 = 0;// Habilitamos la salida del PWM   
}
