//SLAVE2 MOTOR SERVO
//*****************************************************************************
// Palabra de configuración
//*****************************************************************************
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

//*****************************************************************************
// Definición e importación de librerías
//*****************************************************************************
#include <stdint.h>
#include <pic16f887.h>
#include "I2C.h"
#include "setupADC.h"
#include "oscilador.h"
#include <xc.h>
//*****************************************************************************
// Definición de variables
//*****************************************************************************
#define _XTAL_FREQ 500000
uint8_t z;
uint8_t dato;
uint8_t SERVO = 0;



unsigned int ADC_RES; //valor del ADC ccp1
unsigned int valDC; //valor que se carga para el servo
unsigned int valDCL; //adc low
unsigned int valDCH; //adc high
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
void setupPWM(void);                //setup del primer pwm

//void delay(unsigned int micro); //delay para servos
//void setupTMR0(void);               //setup del tmr0

//*****************************************************************************
// Código de Interrupción 
//*****************************************************************************
void __interrupt() isr(void){
   if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            z = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepción
            dato = SSPBUF;
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            //SSPBUF = z;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
   
    //Modificación de valor PWM
   	if (RBIF == 1){
    if (PORTBbits.RB0 == 0)
    {
        __delay_ms(10);
        if (PORTBbits.RB0 == 1){ //incremento el puerto
            if (SERVO != 0){
                SERVO = 0;
            }
            else if (SERVO == 0){
                SERVO = 13;

            }
            INTCONbits.RBIF = 0;
        }
    }
    }
}
//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setupINTOSC(3);
    setup();
    setupPWM();
    //ADC_config(0x01);
    dato = 0;
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
        //PORTD = SERVO;
        CCPR1L = SERVO;  //asigno el valor para el PWM
        __delay_ms(1);             
        
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    
    TRISB = 0b00000111;
    TRISD = 0;
    
    PORTB = 0;
    PORTD = 0;
    
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
    
    I2C_Slave_Init(0x60);   
}

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

