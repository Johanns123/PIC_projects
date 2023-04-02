/*
 * File:   teclado.c
 * Author: Jefferson Henrique
 *
 * Created on 26 de Novembro de 2022, 21:02
 */

#include "main.h"
#include "botoes.h"
#include "lcd.h"

#define DELAY_TECLADO 10

void rotina_teclado(void)
{

}

unsigned char ler_teclado(void) 
{  
    unsigned char entrada, saida = 16;
    
    TRISB = 0b11111000;
    TRISD = 0b00001111;
    
    /*Configura para teclado para coluna_1*/
    LATB |= 0b00000111; 
    LATB ^= 0b00000001;
    for (int i=0; i < DELAY_TECLADO; i++) Nop();
    
    entrada = PORTD;
    
    if ((entrada & 1) == 0) saida = 10; // tecla = *
    if ((entrada & 2) == 0) saida =  7; // tecla = 7
    if ((entrada & 4) == 0) saida =  4; // tecla = 4
    if ((entrada & 8) == 0) saida =  1; // tecla = 1
    
    /*Configura para teclado para coluna_2*/
    LATB ^= 0b00000011;
    for (int i=0; i < DELAY_TECLADO; i++) Nop();
    
    entrada = PORTD;
    
    if ((entrada & 1) == 0) saida = 0; // tecla = 0
    if ((entrada & 2) == 0) saida = 8; // tecla = 8
    if ((entrada & 4) == 0) saida = 5; // tecla = 5
    if ((entrada & 8) == 0) saida = 2; // tecla = 2
    
    /*Configura para teclado para coluna_3*/
    LATB ^= 0b00000110;
    for (int i=0; i < DELAY_TECLADO; i++) Nop();
    
    entrada = PORTD;
    
    if ((entrada & 1) == 0) saida = 11; // tecla = #
    if ((entrada & 2) == 0) saida =  9; // tecla = 9
    if ((entrada & 4) == 0) saida =  6; // tecla = 6
    if ((entrada & 8) == 0) saida =  3; // tecla = 3
    
    /*Volta pra configuração anterior*/
    TRISB = 0b11111111;
    TRISD = 0b00000000;
       
    return saida;
}

bool leitura_botao_pressed(char select)
{
    bool pressed = 0;
    unsigned char dado, botao = 1;
    
    /* Coleta dados para testes */
    dado = PORTB;
    
    /* Testa se botao foi pressionado */
    if (dado & (botao << select))  pressed = !pressed;
    
    return pressed;
}

bool leitura_botao_click(char select)
{
    unsigned char dado, f_cont, botao = 1, click = 0;
    static unsigned char m3_cont = 1, m4_cont, m5_cont;
    
    dado = PORTB;
    
    switch(select)
    {
        case 3:
            if (dado & (botao << select))  f_cont = 0; else f_cont = 1;
            if(f_cont < m3_cont)           click = !click;         
            m3_cont = f_cont;
            break;
        case 4:
            if (dado & (botao << select))  f_cont = 0; else f_cont = 1;
            if(f_cont < m4_cont)           click = !click;         
            m4_cont = f_cont;
            break;
        case 5:
            if (dado & (botao << select))  f_cont = 0; else f_cont = 1;
            if(f_cont < m5_cont)           click = !click;         
            m5_cont = f_cont;
            break;
    }
    return click;
}