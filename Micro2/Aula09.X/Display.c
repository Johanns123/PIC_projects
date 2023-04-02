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

//void cont1_crescente(void)
//{
//    switch (estado_sequencia01) {
//        case 13:
//            display1 = 1;
//            display2 = 3;
//            estado_sequencia01 = 29;
//            break;
//        
//         case 29:
//            display1 = 2;
//            display2 = 9;
//            estado_sequencia01 = 27;
//            break;
//            
//        case 27:
//            display1 = 2;
//            display2 = 7;
//            estado_sequencia01 = 47;
//            break;
//        
//        case 47:
//            display1 = 4;
//            display2 = 7;
//            estado_sequencia01 = 52;
//            break;
//        
//        case 52:
//            display1 = 5;
//            display2 = 2;
//            estado_sequencia01 = 51;
//            break;
//        
//        case 51:
//            display1 = 5;
//            display2 = 1;
//            estado_sequencia01 = 83;
//            break;
//        
//        case 83:
//            display1 = 8;
//            display2 = 3;
//            estado_sequencia01 = 78;
//            break;
//        
//        case 78:
//            display1 = 7;
//            display2 = 8;
//            estado_sequencia01 = 50;
//            break;
//        
//        case 50:
//            display1 = 5;
//            display2 = 0;
//            estado_sequencia01 = 41;
//            break;
//        
//        case 41:
//            display1 = 4;
//            display2 = 1;
//            estado_sequencia01 = 13;
//            break;
//                
//         default:
//            display1 = 2;
//            display2 = 7;
//            estado_sequencia01 = 27;
//            break;
//    }
//}
//
//void cont1_decrescente(void)
//{
//    switch (estado_sequencia01) {
//        case 13:
//            display1 = 1;
//            display2 = 3;
//            estado_sequencia01 = 41;
//            break;
//        
//         case 29:
//            display1 = 2;
//            display2 = 9;
//            estado_sequencia01 = 13;
//            break;
//            
//        case 27:
//            display1 = 2;
//            display2 = 7;
//            estado_sequencia01 = 29;
//            break;
//        
//        case 47:
//            display1 = 4;
//            display2 = 7;
//            estado_sequencia01 = 27;
//            break;
//        
//        case 52:
//            display1 = 5;
//            display2 = 2;
//            estado_sequencia01 = 47;
//            break;
//        
//        case 51:
//            display1 = 5;
//            display2 = 1;
//            estado_sequencia01 = 52;
//            break;
//        
//        case 83:
//            display1 = 8;
//            display2 = 3;
//            estado_sequencia01 = 51;
//            break;
//        
//        case 78:
//            display1 = 7;
//            display2 = 8;
//            estado_sequencia01 = 83;
//            break;
//        
//        case 50:
//            display1 = 5;
//            display2 = 0;
//            estado_sequencia01 = 78;
//            break;
//        
//        case 41:
//            display1 = 4;
//            display2 = 1;
//            estado_sequencia01 = 50;
//            break;
//                
//        default:
//            display1 = 2;
//            display2 = 7;
//            estado_sequencia01 = 27;
//            break;
//    }
//}
//
//void cont2_crescente(void)
//{
//    switch (estado_sequencia02)
//    {
//        case 20:
//            display3 = 2;
//            display4 = 0;       
//            estado_sequencia02 = 12;
//            break;
//            
//        case 12:
//            display3 = 1;
//            display4 = 2;       
//            estado_sequencia02 = 81;
//            break;
//
//        case 81:
//            display3 = 8;
//            display4 = 1;       
//            estado_sequencia02 = 31;
//            break;
//
//        case 31:
//            display3 = 3;
//            display4 = 1;       
//            estado_sequencia02 = 75;
//            break;
//
//        case 75:
//            display3 = 7;
//            display4 = 5;       
//            estado_sequencia02 = 65;
//            break;
//
//        case 65:
//            display3 = 6;
//            display4 = 5;       
//            estado_sequencia02 = 90;
//            break;
//
//        case 90:
//            display3 = 9;
//            display4 = 0;       
//            estado_sequencia02 = 16;
//            break;
//
//        case 16:
//            display3 = 1;
//            display4 = 6;       
//            estado_sequencia02 = 58;
//            break;
//
//        case 58:
//            display3 = 5;
//            display4 = 8;       
//            estado_sequencia02 = 62;
//            break;
//
//        case 62:
//            display3 = 6;
//            display4 = 2;       
//            estado_sequencia02 = 20;
//            break;
//
//        default:
//            display3 = 1;
//            display4 = 6;
//            estado_sequencia02 = 16;
//            break;
//    }
//}
//
//void cont2_decrescente(void)
//{
//    switch (estado_sequencia02)
//    {
//        case 20:
//            display3 = 2;
//            display4 = 0;       
//            estado_sequencia02 = 62;
//            break;
//            
//        case 12:
//            display3 = 1;
//            display4 = 2;       
//            estado_sequencia02 = 20;
//            break;
//
//        case 81:
//            display3 = 8;
//            display4 = 1;       
//            estado_sequencia02 = 12;
//            break;
//
//        case 31:
//            display3 = 3;
//            display4 = 1;       
//            estado_sequencia02 = 81;
//            break;
//
//        case 75:
//            display3 = 7;
//            display4 = 5;       
//            estado_sequencia02 = 31;
//            break;
//
//        case 65:
//            display3 = 6;
//            display4 = 5;       
//            estado_sequencia02 = 75;
//            break;
//
//        case 90:
//            display3 = 9;
//            display4 = 0;       
//            estado_sequencia02 = 65;
//            break;
//
//        case 16:
//            display3 = 1;
//            display4 = 6;       
//            estado_sequencia02 = 90;
//            break;
//
//        case 58:
//            display3 = 5;
//            display4 = 8;       
//            estado_sequencia02 = 16;
//            break;
//
//        case 62:
//            display3 = 6;
//            display4 = 2;       
//            estado_sequencia02 = 58;
//            break;
//
//        default:
//            display3 = 1;
//            display4 = 6;
//            estado_sequencia02 = 16;
//            break;
//    }
//}
