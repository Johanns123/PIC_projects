/*
 * File:   mian.c
 * Author: Johann Steffanno e Guilherme Rocha
 *
 * Created on 27 de Setembro de 2022, 19:20
 */

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <pic18f4520.h>
#include "configbits.h"

#define _XTAL_FREQ  4000000UL
#define TEMP_MIN    200
#define TEMP_MAX    800

//15 14 13...                   //0 1 2...
//contadorH  -> subtrai ate 15 , contadorL -> soma ate 15
//Contar a cada 500ms    contar a cada 900ms

uint8_t contador = 0;
uint8_t estado = 0, led1 = 2, led2 = 2, sel;
uint8_t saida = 0;
uint8_t f_direcao, f_on_off;


void setup(void);
void loop(void);
void atraso_ms(uint16_t tempo);
void f_int0(void);
void f_int1(void);
void f_int2(void);
void f_controle(void);
unsigned char desloca_pra_direita(unsigned char dado);
unsigned char desloca_pra_esquerda(unsigned char dado);

void __interrupt (high_priority) f_interrupt_high (void) //função chamada ao ocorrer uma interrupção
{
    if(INT0IF && INT0IE)
    {
        f_int0();
        INT0IF = 0;
    }
    
    if(INT1IF && INT1IE)
    {
        f_int1();
        INT1IF = 0;
    }
    
    if(INT2IF && INT2IE)
    {
        f_int2();
        INT2IF = 0;
    }
}

int main(void) {
   setup();
   while(1) loop(); 
   return 0;
}


void atraso_ms(uint16_t tempo)
{
    while(tempo)
    {
        tempo--;
        __delay_ms(1);
    }
}

void setup(void) {
    
    IPEN = 0;
    GIE = 0;
    PEIE = 0;
    
    ADCON1  = 0b00001111;
    TRISB   = 0b11111111;
    TRISD   = 0b00000000;
    LATD    = 0b00000000;
    LATB    = 0b00000000;
    INTCON  = 0b00010000;    //habilito a interrupção do INT0
    INTCON2 = 0b00000101;    //habilito a interrupção para borda de descida, e outras interrupções com alta prioridade
    INTCON3 = 0b11011000;    //habilito as interrupções externas e coloco-as em alta prioridade
    
    f_direcao = 0;
    f_on_off = 0;
    led1 = 0b10000000;
    led2 = 0b00000001;
    
    sel = 0;
    
    GIE = 1;
}

void loop(void) {
    
    
    if (estado == 30) 
    {
        estado = 0;
        f_controle();
    }
    else
    {
        estado++;
    }
    
    LATD = saida;
    
    atraso_ms(10);
}

void f_int0(void)
{
    if(sel < 3)
        sel++;
    
    else
        sel = 0;
}

void f_int1(void)
{
    
}

void f_int2(void)
{

}

void f_controle(void)
{
    switch (sel)
    {
        case 0:
            saida = 0x00;
            break;
            
        case 1:
            led1 = desloca_pra_direita(led1);
            led2 = desloca_pra_esquerda(led2);
            saida = led2;
            break;
        
        case 2:
            led1 = desloca_pra_direita(led1);
            led2 = desloca_pra_esquerda(led2);
            saida = led1;
            break;
            
        case 3:
            led1 = desloca_pra_direita(led1);
            led2 = desloca_pra_esquerda(led2);
            saida = led1 | led2;
            break;          
    }
}


unsigned char desloca_pra_direita(unsigned char dado)
{
    //if(dado > 1)
        dado >> 1;
    //else
    //    dado = 128;
    
    return dado;
}

unsigned char desloca_pra_esquerda(unsigned char dado)
{
    //if(dado < 128)
        dado << 1;
    //else
    //    dado = 1;
    
    return dado;
}