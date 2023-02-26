//SLAVE1 MOTOR DC
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
#define _XTAL_FREQ 2000000
#define TMR2PRESCALE 4
uint8_t z;
uint8_t dato;
uint8_t ADC;
uint8_t vel;
unsigned int ADC_RES; //valor del ADC ccp1
unsigned int valDC; //valor que se carga para el servo
unsigned int valDCL; //adc low
unsigned int valDCH; //adc high

uint16_t dutycycle = 0;
uint16_t dutyCycleApply = 0;
const uint32_t pwmFreq = 5000;
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
void setupPWM(void);                //setup del primer pwm
uint32_t pwmMaxDuty(const uint32_t freq);
void initPwm(const uint32_t freq);
void applyPWMDutyCycle(uint16_t dutyCycle, const uint32_t freq);

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
            SSPBUF = ADC;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
   

}
//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setupINTOSC(5); //INTRC A 2MHz
    setup();
    setupPWM();
    initPwm(pwmFreq); // Initialize PWM
    applyPWMDutyCycle(dutycycle,pwmFreq);
    ADC_config(0x01);
    dato = 0;
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){        
        if (z == 1){
            PORTDbits.RD0 = 0;
            PORTDbits.RD1 = 1;
            ADC = ADC_read(0);
            dutycycle = 4*ADC;
            if (dutycycle != dutyCycleApply){
                applyPWMDutyCycle(dutycycle,pwmFreq);
                dutyCycleApply = dutycycle;
            }  
        }
        else if (z == 0){
            dutycycle = 0;
            if (dutycycle != dutyCycleApply){
                applyPWMDutyCycle(dutycycle,pwmFreq);
                dutyCycleApply = dutycycle; 
            }  
        }    
    }   
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;

    
    TRISB = 0b00000000;
    TRISD = 0;
    
    PORTB = 0;
    PORTC = 0; 
    PORTD = 0;
    
    //Interrupciones
    INTCONbits.GIE = 1; //interrupciones globales

    I2C_Slave_Init(0x50);   
}

void setupPWM(void){
    // Paso 1
    TRISCbits.TRISC2 = 1; 
    // Paso 2
    //PR2 = 249;      // Periodo de 20mS  
    // Paso 3
    CCP1CON = 0b00001100;        // P1A como PWM 
   // Paso 4
    //CCP1CONbits.DC1B = valDCL;        // CCPxCON<5:4>
    //CCPR1L = 0;        // CCPR1L   
    // Paso 5
    TMR2IF = 0;
    T2CONbits.T2CKPS = 0b01;      // Prescaler de 1:16
    TMR2ON = 1;         // Encender timer 2 
    // Paso 6
    while(!TMR2IF);
    TRISCbits.TRISC2 = 0;// Habilitamos la salida del PWM   
}

uint32_t pwmMaxDuty(const uint32_t freq)
{
  return(_XTAL_FREQ/(freq*TMR2PRESCALE));
}
//Calculate the PR2 value
void initPwm(const uint32_t freq)
{
    //calculate period register value
    PR2 = (uint8_t)((_XTAL_FREQ/(freq*4*TMR2PRESCALE)) - 1);
}

//Give a value in between 0 and 1024 for duty-cycle
void applyPWMDutyCycle(uint16_t dutyCycle, const uint32_t freq)
{
    if(dutyCycle<1024)
    {
        //1023 because 10 bit resolution
        dutyCycle = (uint16_t)(((float)dutyCycle/1023)*pwmMaxDuty(freq));
        CCP1CON &= 0xCF;                 // Make bit4 and 5 zero (Store fraction part of duty cycle)
        CCP1CON |= (0x30&(dutyCycle<<4)); // Assign Last 2 LSBs to CCP1CON
        CCPR1L = (uint8_t)(dutyCycle>>2); // Put MSB 8 bits in CCPR1L
    }
}