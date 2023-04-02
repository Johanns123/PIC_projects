/*
 * File:   Displays.c
 * Author: johanns123
 *
 * Created on 1 de Novembro de 2022, 23:46
 */


#include "Display.h"

/*Faço a multiplexação dos displays para dar um efeito visual de paralelismo*/
uint8_t estado_sequencia01 = 0, estado_sequencia02 = 0;

void mux_display() {
    static uint8_t estado = 0;
    
    //ADCON1 = 0b00001101;    //pino AN0 e AN1 como AD
    
    switch (estado) 
    {
        case 0: //acendo o display 1 e apago os outros
            disp1 = 1;
            disp4 = 0;
            display = conv7seg[display1]; //passo para os segmentos do display o número a ser mostrado
            estado = 1;
            break;

        case 1: //acendo o display 2 e apago os outros
            disp1 = 0;
            disp2 = 1;
            display = conv7seg[display2];
            estado = 2;
            break;

        case 2: //acendo o display 3 e apago os outros
            disp2 = 0;
            disp3 = 1;
            display = conv7seg[display3];
            estado = 3;
            break;

        case 3: //acendo o display 4 e apago os outros
            disp3 = 0;
            disp4 = 1;
            display = conv7seg[display4];
            estado = 0;
            break;
    }
}