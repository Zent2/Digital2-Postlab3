/* 
 * File:   mainPostLab.c
 * Author: Christian Alessandro Campos López 21760
 *
 * Created on 30 de julio de 2023, 05:05 PM
 */


//******************************************************************************
// Palabra de configuración
//******************************************************************************
// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
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

//******************************************************************************
// Librerías Generales
//******************************************************************************
#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include <pic16f887.h>
//******************************************************************************
// Librerías Propias
//******************************************************************************
#include "lcd8.h"
#include "map.h"
#include "SPI.h"


//******************************************************************************
// Variables
//******************************************************************************
#define _XTAL_FREQ 8000000     // Frecuencia de oscilador de 8MHz
uint8_t contador, pot1, pot2;
uint8_t contador_pasado=0; 
uint8_t pot1_pasado=0; 
uint8_t pot2_pasado=0;
char contadorR[3];
char pot1R[3];
char pot2R[3];
uint8_t cont=0;
uint8_t bandera=0;

//******************************************************************************
// Prototipos de función
//******************************************************************************
void setup(void);
//******************************************************************************
// Vector de Interrupción
//******************************************************************************

void __interrupt() isr(void) {
    return;
}
//******************************************************************************
// Función principal
//******************************************************************************

void main(void) {
    setup(); // Inicializar el microcontrolador

    while (1) {
        if (cont==0){
            Lcd_Clear();
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("POSTLAB3");
            __delay_us(5);
            Lcd_Set_Cursor(2,1);
            Lcd_Write_String("SPI");
            cont=1;
            __delay_ms(500);
        }
        
        PORTEbits.RE2 = 0;       //Slave Select
       __delay_ms(1);
       
       spiWrite(0);
       contador = spiRead();
       
       __delay_ms(1);
       PORTEbits.RE2 = 1;       //Slave Deselect 
       
       PORTEbits.RE1 = 0;       //Slave Select
       __delay_ms(1);
       
       spiWrite(0);
       pot2 = spiRead();
       
       __delay_ms(1);
       PORTEbits.RE1 = 1;       //Slave Deselect 
       
       PORTEbits.RE2 = 0;       //Slave Select
       __delay_ms(1);
       
       spiWrite(0);
       pot1 = spiRead();
       
       __delay_ms(1);
       PORTEbits.RE2 = 1;       //Slave Deselect 
       
       __delay_ms(250);
       
       if (contador != contador_pasado){
       contadorR[0] = (contador / 100) + '0'; // Convertir centenas a carácter ASCII
       contadorR[1] = ((contador / 10) % 10) + '0'; // Convertir decenas a carácter ASCII
       contadorR[2] = (contador % 10) + '0'; // Convertir unidades a carácter ASCII
       contador_pasado=contador;
       bandera=1;
       }
       if (pot1 != pot1_pasado){
       pot1R[0] = (pot1 / 100) + '0'; // Convertir centenas a carácter ASCII
       pot1R[1] = ((pot1 / 10) % 10) + '0'; // Convertir decenas a carácter ASCII
       pot1R[2] = (pot1 % 10) + '0'; // Convertir unidades a carácter ASCII
       pot1_pasado=pot1;
       bandera=1;
       }
       if (pot2 != pot2_pasado){
       
       pot2R[0] = (pot2 / 100) + '0'; // Convertir centenas a carácter ASCII
       pot2R[1] = ((pot2 / 10) % 10) + '0'; // Convertir decenas a carácter ASCII
       pot2R[2] = (pot2 % 10) + '0'; // Convertir unidades a carácter ASCII
       pot2_pasado=pot2;
       bandera=1;
       }
       if (bandera==1){
            Lcd_Clear();
            Lcd_Set_Cursor(1,1);
            Lcd_Write_String("CONT POT1 POT2");
            __delay_us(5);
            Lcd_Set_Cursor(2,1);
            Lcd_Write_Char(contadorR[0]); // Mostrar centenas
            Lcd_Write_Char(contadorR[1]); // Mostrar decenas
            Lcd_Write_Char(contadorR[2]); // Mostrar unidades
            Lcd_Write_String(" ");
            Lcd_Write_Char(pot1R[0]); // Mostrar centenas
            Lcd_Write_Char(pot1R[1]); // Mostrar decenas
            Lcd_Write_Char(pot1R[2]); // Mostrar unidades
            Lcd_Write_String(" ");
            Lcd_Write_Char(pot2R[0]); // Mostrar centenas
            Lcd_Write_Char(pot2R[1]); // Mostrar decenas
            Lcd_Write_Char(pot2R[2]); // Mostrar unidades
            bandera=0;       
       }
       
       
    }
}
//******************************************************************************
// Funciones secundarias
//******************************************************************************

void setup(void) {
    // Configuraci�n de los puertos
    ANSEL=0;
    ANSELH=0;
    TRISA=0;
    TRISD=0;
    PORTD=0;
    PORTA=0;
    TRISE = 0;
    PORTEbits.RE1 = 1;
    PORTEbits.RE2 = 1;
    TRISB=0x00;
    
    
    //Configuraci�n LCD
    Lcd_Init();
    
    //Iniciar SPI
    spiInit(SPI_MASTER_OSC_DIV4, SPI_DATA_SAMPLE_MIDDLE, SPI_CLOCK_IDLE_LOW, SPI_IDLE_2_ACTIVE);
    //Oscilador a 8MHz
    OSCCONbits.IRCF = 0b111;
    OSCCONbits.SCS = 1;


}