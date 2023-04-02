/*
 * File:   teclado.c
 * Author: hd-portable
 *
 * Created on 21 de Novembro de 2022, 13:58
 */


#include <xc.h>
#include <stdint.h>
#include "teclado.h"

#define delay_teclado 10

uint8_t ler_teclado(void) 
{
    
    uint8_t entrada, saida=16;
    
    TRISB = 0b11111000;
    TRISD = 0b00001111;
    
    LATB |= 0b00000111; 
    
    LATB ^= 0b00000001;
    for (int i=0; i<delay_teclado; i++) Nop();
    entrada = PORTD;
    
    if ((entrada & 1) == 0) saida=10;
    if ((entrada & 2) == 0) saida=7;
    if ((entrada & 4) == 0) saida=4;
    if ((entrada & 8) == 0) saida=1;
    
    LATB ^= 0b00000011;
    for (int i=0; i<delay_teclado; i++) Nop();
    entrada = PORTD;
    
    if ((entrada & 1) == 0) saida=0;
    if ((entrada & 2) == 0) saida=8;
    if ((entrada & 4) == 0) saida=5;
    if ((entrada & 8) == 0) saida=2;
    
    LATB ^= 0b00000110;
    for (int i=0; i<delay_teclado; i++) Nop();
    entrada = PORTD;
    
    if ((entrada & 1) == 0) saida=11;
    if ((entrada & 2) == 0) saida=9;
    if ((entrada & 4) == 0) saida=6;
    if ((entrada & 8) == 0) saida=3;
    
    TRISB = 0b11111111;
    TRISD = 0b00000000;
       
    return saida;
}
