/*
 * File:   disp_7seg.c
 * Author: Jefferson Henrique
 *
 * Created on 30 de Outubro de 2022, 09:07
 */

#include "main.h"           // recursos globais
#include "disp_7seg.h"      // ações do display_7_segmentos

unsigned char conv7seg[17] =      //conversor pro display de 7 segmentos 
{
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7 
    0b01111111, // 8
    0b01101111, // 9
    0b01110111, // A = 10
    0b01111100, // B = 11
    0b00111001, // C = 12
    0b01011110, // D = 13
    0b01111001, // E = 14
    0b01110001, // F = 15
    0b00000000  // OFF
};

/*Faço a multiplexação dos displays para dar um efeito visual de paralelismo*/
void mux_display()
{
    if(!set_portd)
    {
        static unsigned char estado = 0;

        switch(estado)
        {
        case 0:     //acendo o display 1 e apago os outros
            DISP1 = 1;
            DISP4 = 0;
            DISPLAY = conv7seg[display3];
            estado = 1;
            break;
        
        case 1: //acendo o display 2 e apago os outros
            DISP1 = 0;
            DISP2 = 1;
            DISPLAY = conv7seg[display4];
            estado = 2;
            break;
        
        case 2: //acendo o display 3 e apago os outros
            DISP2 = 0;
            DISP3 = 1;
            DISPLAY = conv7seg[display1];
            estado = 3;
            break;
        
        case 3: //acendo o display 4 e apago os outros
            DISP3 = 0;
            DISP4 = 1;
            DISPLAY = conv7seg[display2];
            estado = 0;
            break;
        }
    }
    else DISP1 = 0, DISP2 = 0, DISP3 = 0, DISP4 = 0;
}

/*-----------Contador de 0 a 99*---------*/
void show_displays_12(void)
{   
    if(!set_portd)
    {
        switch(contagem_dec())
        {
            case 0:
                display2 = 0;
                display1 = 0;       
                break;
            case 1:
                display2 = 1;
                display1 = 0;
                break;
            case 2:
                display2 = 2;
                display1 = 0;
                break;
            case 3:
                display2 = 4;
                display1 = 0;
                break;
            case 4:
                display2 = 5;
                display1 = 0;
                break;  
            case 5:
                display2 = 7;
                display1 = 0;
                break;
            case 6:
                display2 = 8;
                display1 = 0;
                break;
            case 7:
                display2 = 0;
                display1 = 1;
                break;
            case 8:
                display2 = 1;
                display1 = 1;
                break;
            case 9:
                display2 = 3;
                display1 = 1;
                break;
            case 10:
                display2 = 4;
                display1 = 1;
                break;
            case 11:
                display2 = 6;
                display1 = 1;
                break;
            case 12:
                display2 = 7;
                display1 = 1;
                break;
            case 13:
                display2 = 9;
                display1 = 1;
                break;
            case 14:
                display2 = 0;
                display1 = 2;
                break;
        }
    }
}//end contador_displays

void show_displays_34(void)
{   
    if(!set_portd)
    {
        switch(contagem_hex())
        {
        case 0:
            display4 = 0;
            display3 = 0;       
            break;
        case 1:
            display4 = 1;
            display3 = 0;
            break;
        case 2:
            display4 = 4;
            display3 = 0;
            break;
        case 3:
            display4 = 6;   
            display3 = 0;
            break;
        case 4:
            display4 = 8;   
            display3 = 0;
            break;  
        case 5:
            display4 = 9;   
            display3 = 0;
            break;
        case 6:
            display4 = 10;   
            display3 = 0;
            break;
        case 7:
            display4 = 12;
            display3 = 0;
            break;
        case 8:
            display4 = 14;
            display3 = 0;
            break;
        case 9:
            display4 = 15;
            display3 = 0;
            break;
        case 10:
            display4 = 0;
            display3 = 1;
            break;
        case 11:
            display4 = 2;
            display3 = 1;
            break;
        case 12:
            display4 = 4;
            display3 = 1;
            break;
        }
    }    
}//end contador_displays

unsigned char contagem_dec(void)
{
    static unsigned char cont_dec;
    
    if(flag_b0)
    {
        if(flag_b1)
        {
            /*cont_dec_cresc*/
            if(cont_dec < 14)
            {
              cont_dec++;  
            }
            else cont_dec = 0;
        }  
        else
        {
            /*cont_dec_decresc*/
            if(cont_dec < 15)
            {
              cont_dec--;  
            }
            else cont_dec = 14;
        }
    }
    return cont_dec;
}

unsigned char contagem_hex(void)
{
    static unsigned char cont_hex;
    
    if(flag_b0)
    {
        if(flag_b2)
        {
            /*cont_hex*/
            if(cont_hex < 12)
            {
              cont_hex++;  
            }
            else cont_hex = 0; 
        }  
        else
        {
            /*cont_hex_decresc*/
            if(cont_hex < 13)
            {
              cont_hex--;  
            }
            else cont_hex = 12; 
        }
    }
    return cont_hex;
}