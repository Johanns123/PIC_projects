/*
 * File:   Displays.c
 * Author: johanns123
 *
 * Created on 1 de Novembro de 2022, 23:46
 */


#include "Displays.h"

/*Faço a multiplexação dos displays para dar um efeito visual de paralelismo*/
uint8_t estado_sequencia01 = 1, estado_sequencia02 = 0;

void mux_display() {
    static uint8_t estado = 0;

    switch (estado) {
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

void cont1_crescente(void)
{
     switch (estado_sequencia01) {
        case 1:
            display1 = 0;
            display2 = 1;
            estado_sequencia01 = 2;
            break;

        case 2:
            display1 = 0;
            display2 = 2;
            estado_sequencia01 = 4;
            break;

        case 4:
            display1 = 0;
            display2 = 4;
            estado_sequencia01 = 5;
            break;
            
        case 5:
            display1 = 0;
            display2 = 5;
            estado_sequencia01 = 7;
            break;

        case 7:
            display1 = 0;
            display2 = 7;
            estado_sequencia01 = 8;
            break;
        
        case 8:
            display1 = 0;
            display2 = 8;
            estado_sequencia01 = 10;
            break;
        
        case 10:
            display1 = 1;
            display2 = 0;
            estado_sequencia01 = 11;
            break;
        
        case 11:
            display1 = 1;
            display2 = 1;
            estado_sequencia01 = 13;
            break;
        
        case 13:
            display1 = 1;
            display2 = 3;
            estado_sequencia01 = 14;
            break;
        
        case 14:
            display1 = 1;
            display2 = 4;
            estado_sequencia01 = 16;
            break;
        
        case 16:
            display1 = 1;
            display2 = 6;
            estado_sequencia01 = 17;
            break;
            
         case 17:
            display1 = 1;
            display2 = 7;
            estado_sequencia01 = 19;
            break;
            
         case 19:
            display1 = 1;
            display2 = 9;
            estado_sequencia01 = 20;
            break;
        
        case 20:
            display1 = 2;
            display2 = 0;
            estado_sequencia01 = 1;
            break;
            
        default:
            display1 = 0;
            display2 = 0;
            estado_sequencia01 = 0;
            break;
    }
}

void cont1_decrescente(void)
{
     switch (estado_sequencia01) {

        case 1:
            display1 = 0;
            display2 = 1;
            estado_sequencia01 = 20;
            break;

        case 2:
            display1 = 0;
            display2 = 2;
            estado_sequencia01 = 1;
            break;

        case 4:
            display1 = 0;
            display2 = 4;
            estado_sequencia01 = 2;
            break;
            
        case 5:
            display1 = 0;
            display2 = 5;
            estado_sequencia01 = 4;
            break;

        case 7:
            display1 = 0;
            display2 = 7;
            estado_sequencia01 = 5;
            break;
        
        case 8:
            display1 = 0;
            display2 = 8;
            estado_sequencia01 = 7;
            break;
        
        case 10:
            display1 = 1;
            display2 = 0;
            estado_sequencia01 = 8;
            break;
        
        case 11:
            display1 = 1;
            display2 = 1;
            estado_sequencia01 = 10;
            break;
        
        case 13:
            display1 = 1;
            display2 = 3;
            estado_sequencia01 = 11;
            break;
        
        case 14:
            display1 = 1;
            display2 = 4;
            estado_sequencia01 = 13;
            break;
        
        case 16:
            display1 = 1;
            display2 = 6;
            estado_sequencia01 = 14;
            break;
            
         case 17:
            display1 = 1;
            display2 = 7;
            estado_sequencia01 = 16;
            break;
            
         case 19:
            display1 = 1;
            display2 = 9;
            estado_sequencia01 = 17;
            break;
        
        case 20:
            display1 = 2;
            display2 = 0;
            estado_sequencia01 = 19;
            break;
            
        default:
            display1 = 0;
            display2 = 0;
            estado_sequencia01 = 20;
            break;
    }
}

void cont2_crescente(void)
{
    switch (estado_sequencia02)
    {
        case 0:
            display3 = 0;
            display4 = 0;       
            estado_sequencia02 = 1;
            break;
            
        case 1:
            display3 = 0;
            display4 = 1;       
            estado_sequencia02 = 4;
            break;

        case 4:
            display3 = 0;
            display4 = 4;
            estado_sequencia02 = 6;
            break;

        case 6:
            display3 = 0;
            display4 = 6;
            estado_sequencia02 = 8;
            break;

        case 8:
            display3 = 0;
            display4 = 8;
            estado_sequencia02 = 9;
            break;
            
        case 9:
            display3 = 0;
            display4 = 9;
            estado_sequencia02 = 10;
            break;

        case 10:
            display3 = 0;
            display4 = 10;
            estado_sequencia02 = 12;
            break;
        
        case 12:
            display3 = 0;
            display4 = 12;
            estado_sequencia02 = 14;
            break;
        
        case 14:
            display3 = 0;
            display4 = 14;
            estado_sequencia02 = 15;
            break;
        
        case 15:
            display3 = 0;
            display4 = 15;
            estado_sequencia02 = 16;
            break;
        
        case 16:
            display3 = 1;
            display4 = 0;
            estado_sequencia02 = 18;
            break;
        
        case 18:
            display3 = 1;
            display4 = 2;
            estado_sequencia02 = 20;
            break;
        
        case 20:
            display3 = 1;
            display4 = 4;
            estado_sequencia02 = 0;
            break;
            
        default:
            display3 = 0;
            display4 = 0;
            estado_sequencia02 = 0;
            break;
    }
}

void cont2_decrescente(void)
{
    switch (estado_sequencia02)
    {
        case 0:
            display3 = 0;
            display4 = 0;       
            estado_sequencia02 = 20;
            break;
            
        case 1:
            display3 = 0;
            display4 = 1;       
            estado_sequencia02 = 0;
            break;
        
        case 4:
            display3 = 0;
            display4 = 4;
            estado_sequencia02 = 1;
            break;

        case 6:
            display3 = 0;
            display4 = 6;
            estado_sequencia02 = 4;
            break;

        case 8:
            display3 = 0;
            display4 = 8;
            estado_sequencia02 = 6;
            break;
            
        case 9:
            display3 = 0;
            display4 = 9;
            estado_sequencia02 = 8;
            break;

        case 10:
            display3 = 0;
            display4 = 10;
            estado_sequencia02 = 9;
            break;
        
        case 12:
            display3 = 0;
            display4 = 12;
            estado_sequencia02 = 10;
            break;
        
        case 14:
            display3 = 0;
            display4 = 14;
            estado_sequencia02 = 12;
            break;
        
        case 15:
            display3 = 0;
            display4 = 15;
            estado_sequencia02 = 14;
            break;
        
        case 16:
            display3 = 1;
            display4 = 0;
            estado_sequencia02 = 15;
            break;
        
        case 18:
            display3 = 1;
            display4 = 2;
            estado_sequencia02 = 16;
            break;
        
        case 20:
            display3 = 1;
            display4 = 4;
            estado_sequencia02 = 18;
            break;
            
        default:
            display3 = 0;
            display4 = 0;
            estado_sequencia02 = 20;
            break;
    }
}
