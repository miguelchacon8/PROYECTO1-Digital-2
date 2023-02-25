//MASTER PROYECTO 
/*
 * File:   main.c
 * Author: Miguel Chacón
 * Programa: PROYECTO 1
 * Fecha de creación: 09/02/2023
 */

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include "I2C.h"
#include "oscilador.h"

//#include "mapf.h"


#include <pic16f887.h>
#include <xc.h>
#include <stdio.h>
#include <string.h>

#define _XTAL_FREQ 8000000 //8MHz

//*****************************************************************************
// Definición de funciones 
//*****************************************************************************

void setup(void);
//void valorLCD(void);
//void calculovolt(void);
//void modoboton(void);
//void verificacion(void);

//*****************************************************************************
// Definición de variables 
//*****************************************************************************

//variables para botones
uint8_t iniciostop = 0;
uint8_t cambio = 0;



//*****************************************************************************
// Código de Interrupción 
//*****************************************************************************
void __interrupt() ISR (void){

}
//******************************************************************************
// Código Principal
//******************************************************************************
void main(void) {
    setupINTOSC(7);         //Oscilador a 8MHz
    setup();

    while(1){ 
        //LECTURA DEL ADC
//        I2C_Master_Start();
//        I2C_Master_Write(0x51);
//        V1 = I2C_Master_Read(0);
//        I2C_Master_Stop();
//        __delay_ms(20);

    }
}
//******************************************************************************
// Función para configurar GPIOs
//******************************************************************************
void setup(void){
    //PUERTOS
    ANSEL = 0;
    ANSELH = 0;
    TRISB = 0b00000111;
    TRISA = 0;
    TRISD = 0;
    
    PORTA = 0; //inicializar puertos
    PORTB = 0;
    PORTD = 0;
    PORTE = 0;
    
        //Interrupciones
    INTCONbits.RBIE = 1; 
    INTCONbits.RBIF = 0;
    INTCONbits.GIE = 1; //interrupciones globales
    
    WPUBbits.WPUB0 = 1; //inputs
    WPUBbits.WPUB1 = 1;
    IOCBbits.IOCB0 = 1; //inputs
    IOCBbits.IOCB1 = 1;
    WPUBbits.WPUB2 = 1;
    IOCBbits.IOCB2 = 1; //inputs
    
    OPTION_REGbits.nRBPU = 0; //no RBPU, habilitan los pullups internos
    
    I2C_Master_Init(100000); //Se inicializa la comunicación I2C
}

//******************************************************************************
// Función para calcular valores de unidades y decenas:
//******************************************************************************

