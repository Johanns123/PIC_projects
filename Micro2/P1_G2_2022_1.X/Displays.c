/*
 * File:   Displays.c
 * Author: johanns123
 *
 * Created on 1 de Novembro de 2022, 23:46
 */


#include "Displays.h"

/*Faço a multiplexação dos displays para dar um efeito visual de paralelismo*/
uint8_t estado_sequencia01 = 0, estado_sequencia02 = 0;

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
        case 0:
            display3 = 0;
            display2 = 0;
            estado_sequencia01 = 63;
            break;
        
         case 63:
            display3 = 3;
            display2 = 6;
            estado_sequencia01 = 46;
            break;
            
        case 46:
            display3 = 6;
            display2 = 4;
            estado_sequencia01 = 15;
            break;
        
        case 15:
            display3 = 5;
            display2 = 1;
            estado_sequencia01 = 25;
            break;
        
        case 25:
            display3 = 5;
            display2 = 2;
            estado_sequencia01 = 60;
            break;
        
        case 60:
            display3 = 0;
            display2 = 6;
            estado_sequencia01 = 12;
            break;
        
        case 12:
            display3 = 2;
            display2 = 1;
            estado_sequencia01 = 64;
            break;
        
        case 64:
            display3 = 4;
            display2 = 6;
            estado_sequencia01 = 47;
            break;
        
        case 47:
            display3 = 7;
            display2 = 4;
            estado_sequencia01 = 74;
            break;
        
        case 74:
            display3 = 4;
            display2 = 7;
            estado_sequencia01 = 68;
            break;
        
        case 68:
            display3 = 8;
            display2 = 6;
            estado_sequencia01 = 28;
            break;
        
        case 28:
            display3 = 8;
            display2 = 2;
            estado_sequencia01 = 82;
            break;
            
        case 82:
            display3 = 2;
            display2 = 8;
            estado_sequencia01 = 0;
            break;  
            
         default:
            display3 = 0;
            display2 = 0;
            estado_sequencia01 = 0;
            break;
    }
}

void cont1_decrescente(void)
{
    switch (estado_sequencia01) {
        case 0:
            display3 = 0;
            display2 = 0;
            estado_sequencia01 = 82;
            break;
        
         case 63:
            display3 = 3;
            display2 = 6;
            estado_sequencia01 = 0;
            break;
            
        case 46:
            display3 = 6;
            display2 = 4;
            estado_sequencia01 = 63;
            break;
        
        case 15:
            display3 = 5;
            display2 = 1;
            estado_sequencia01 = 46;
            break;
        
        case 25:
            display3 = 5;
            display2 = 2;
            estado_sequencia01 = 15;
            break;
        
        case 60:
            display3 = 0;
            display2 = 6;
            estado_sequencia01 = 25;
            break;
        
        case 12:
            display3 = 2;
            display2 = 1;
            estado_sequencia01 = 60;
            break;
        
        case 64:
            display3 = 4;
            display2 = 6;
            estado_sequencia01 = 12;
            break;
        
        case 47:
            display3 = 7;
            display2 = 4;
            estado_sequencia01 = 64;
            break;
        
        case 74:
            display3 = 4;
            display2 = 7;
            estado_sequencia01 = 47;
            break;
        
        case 68:
            display3 = 8;
            display2 = 6;
            estado_sequencia01 = 74;
            break;
        
        case 28:
            display3 = 8;
            display2 = 2;
            estado_sequencia01 = 68;
            break;
            
        case 82:
            display3 = 2;
            display2 = 8;
            estado_sequencia01 = 28;
            break;  
            
         default:
            display3 = 0;
            display2 = 0;
            estado_sequencia01 = 0;
            break;
    }
}

void cont2_crescente(void)
{
    switch (estado_sequencia02)
    {
        case 0:
            display4 = 0;       
            estado_sequencia02 = 12;
            break;
            
        case 12:
            display4 = 12;       
            estado_sequencia02 = 8;
            break;

        case 8:
            display4 = 8;       
            estado_sequencia02 = 9;
            break;

        case 9:
            display4 = 9;       
            estado_sequencia02 = 14;
            break;

        case 14:
            display4 = 14;       
            estado_sequencia02 = 1;
            break;

        case 1:
            display4 = 1;       
            estado_sequencia02 = 5;
            break;

        case 5:
            display4 = 5;       
            estado_sequencia02 = 10;
            break;

        case 10:
            display4 = 10;       
            estado_sequencia02 = 4;
            break;

        case 4:
            display4 = 4;       
            estado_sequencia02 = 15;
            break;

        case 15:
            display4 = 15;       
            estado_sequencia02 = 3;
            break;

        case 3:
            display4 = 3;       
            estado_sequencia02 = 6;
            break;

        case 6:
            display4 = 6;       
            estado_sequencia02 = 2;
            break;
        
        case 2:
            display4 = 2;       
            estado_sequencia02 = 0;
            break;

        default:
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
            display4 = 0;       
            estado_sequencia02 = 2;
            break;
            
        case 12:
            display4 = 12;       
            estado_sequencia02 = 0;
            break;

        case 8:
            display4 = 8;       
            estado_sequencia02 = 12;
            break;

        case 9:
            display4 = 9;       
            estado_sequencia02 = 8;
            break;

        case 14:
            display4 = 14;       
            estado_sequencia02 = 9;
            break;

        case 1:
            display4 = 1;       
            estado_sequencia02 = 14;
            break;

        case 5:
            display4 = 5;       
            estado_sequencia02 = 1;
            break;

        case 10:
            display4 = 10;       
            estado_sequencia02 = 5;
            break;

        case 4:
            display4 = 4;       
            estado_sequencia02 = 10;
            break;

        case 15:
            display4 = 15;       
            estado_sequencia02 = 4;
            break;

        case 3:
            display4 = 3;       
            estado_sequencia02 = 15;
            break;

        case 6:
            display4 = 6;       
            estado_sequencia02 = 3;
            break;
        
        case 2:
            display4 = 2;       
            estado_sequencia02 = 6;
            break;

        default:
            display4 = 0;
            estado_sequencia02 = 0;
            break;
    }
}
