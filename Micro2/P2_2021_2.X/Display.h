#ifndef DISPLAYS
#define	DISPLAYS

#include <xc.h> // include processor files - each processor file is guarded.  
#define disp1 LATA2
#define disp2 LATA3
#define disp3 LATA4
#define disp4 LATA5

#define display LATD
uint8_t display1 = 0, display2 = 0, display3 = 0, display4 = 0;
uint8_t conv7seg[17] = //conversor pro display de 7 segmentos 
{
    0b00111111,
    0b00000110,
    0b01011011,
    0b01001111,
    0b01100110,
    0b01101101,
    0b01111101,
    0b00000111,
    0b01111111,
    0b01101111,
    0b01110111,
    0b01111100,
    0b00111001,
    0b01011110,
    0b01111001,
    0b01110001,
    0b00000000
};
void mux_display(void);
//void cont1_crescente(void);
//void cont1_decrescente(void);
//void cont2_crescente(void);
//void cont2_decrescente(void);

#endif