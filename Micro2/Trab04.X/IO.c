/*
 * File:   Hardware.c
 * Author: johanns123
 *
 * Created on 1 de Novembro de 2022, 23:56
 */

#include "IO.h"
#include "lcd.h"
#include "Display.h"
#include "ADC.h"
#include "Interrupt.h"

void setup(void)
{
    disable_interrupts();
    setup_io(); //defino os dispositivos de entrada e saída
    interrupt_init();
    T2CON  = 0b00000111;    //PS de 16
    PR2 = 155;              //PR2 referente a FPWM de 1kHZ com FMAQ = 2.5MHz
    CCP1CON = 0b00001100;
    AD_init();
    disp1 = 1, disp2 = 1, disp3 = 1, disp4 = 1;
    lcd_init();
    display1 = 0;
    display2 = 0;
    display3 = 0;
    display4 = 0;
    enable_interrupts();
    start_AD_conversion();
}


void setup_io(void) 
{
    TRISA = 0b11000011;
    TRISB = 0b00111111;
    TRISC = 0b11011011;
    TRISD = 0b00000000;
    TRISE = 0b11111000;
    LATA  = 0b00000000;
    LATB  = 0b00000000;
    LATC  = 0b00100000; 
    LATD  = 0b00000000;
}