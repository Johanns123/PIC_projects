/*
 * File:   Hardware.c
 * Author: johanns123
 *
 * Created on 1 de Novembro de 2022, 23:56
 */

#include "IOs.h"
#include "ADC.h"
#include "Interrupts.h"
#include "Displays.h"

void setup(void) {
    disable_interrupts();
    setup_io(); //defino os dispositivos de entrada e saída
    interrupt_init();
    AD_init();
    disp1 = 1, disp2 = 1, disp3 = 1, disp4 = 1;
    temporizador1 = 300;
    temporizador2 = 500;
    start_AD_conversion();
    enable_interrupts();
}


void setup_io(void) {
    TRISA = 0b11000011; //RA5 -> RA2 como saida 
    TRISB = 0b00011111; //RB0 -> RB4 como entrada
    TRISD = 0b00000000; //tudo como saida
    LATA = 0b00000000; //Inicio em nível baixo
    LATD = 0b00000000; //Inicio em nível baixo
    LATB = 0b00000000; //Inicio em nível baixo
}