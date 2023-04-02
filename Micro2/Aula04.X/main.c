/*
 * File:   mian.c
 * Author: Johann Steffanno e Guilherme Rocha
 *
 * Created on 27 de Setembro de 2022, 19:20
 */

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "configbits.h"

#define _XTAL_FREQ  4000000UL
#define TEMP_MIN    30
#define TEMP_MAX    800

#define disp1 LATA2
#define disp2 LATA3
#define disp3 LATA4
#define disp4 LATA5

#define display LATD

uint8_t conv7seg[17] = {
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

uint8_t contador = 0;
uint8_t estado = 0, led1, led2, sel;
uint8_t saida = 0;


void setup(void);
void setup_io(void);
void loop(void);
void atraso_ms(uint16_t tempo);
void f_int0(void);
void f_int1(void);
void f_int2(void);
void f_controle(void);
void f_timers(void);
uint8_t desloca_pra_direita(uint8_t dado);
uint8_t desloca_pra_esquerda(uint8_t dado);

void __interrupt (high_priority) f_interrupt_high (void) //função chamada ao ocorrer uma interrupção
{
    if(TMR0IF)
    {          //Testando se houve estouro no Timer0
        TMR0 = 0x06;                //Reinicia o registrador Timer0
        
        f_timers();
        
        TMR0IF = 0x00;   //limpa a flag para a próxima interrupção
    }
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

void f_timers(void)
{
    static uint16_t counter0;
    if(counter0 < 500)
        {  
            counter0++;                  //Incrementa o counter
        }
        
        else 
        {
            //LATD ^= (1<<4);
            counter0 = 1;
        }
}

void setup(void) {
    
    IPEN = 0;       //desabilito as interrupções por prioridade
    GIE = 0;        //desabilito a chave de interrupção global
    PEIE = 0;       //Desabilito a chave de interrupção dos periféricos
    
    setup_io();
    
    INTCON  = 0b10010000;    //habilito a interrupção do INT0
    INTCON2 = 0b00000101;    //habilito a interrupção para borda de descida, e outras interrupções com alta prioridade
    INTCON3 = 0b11011000;    //habilito as interrupções externas e coloco-as em alta prioridade
    
    T0CON = 0b11000001;
    //configura o TMR0 como contador de 8 bits
    //clock externo
    //associa os pre scaler ao módulo timer0
    //Define o valor do prescaler no caso em 1:4 --> 1 no T0PS0 associado ao tmr0
    
    TMR0 = 0x06;                        //inicia a contagem em 6, tempo de 1ms
    
    //led1 = 0b10000000;
    //led2 = 0b00000001;
    
    //sel = 0;
    
    disp1 = 1, disp2 = 1, disp3 = 1, disp4 = 1;
    
    
    TMR0IE = 1;          //Habilita ainterrupção por estouro do Timer0
    GIE = 1;
}

void setup_io(void)
{
    ADCON1  = 0b00001111;   //pinos com AD desconfigurado
    TRISA   = 0b11000011;   //RA5 -> RA2 como saida 
    TRISB   = 0b00000011;   //tudo como entrada
    TRISD   = 0b00000000;   //tudo como saida
    LATA    = 0b00000000;
    LATD    = 0b00000000;
    LATB    = 0b00000000;
}

void loop(void) 
{    
    /*if (estado == TEMP_MIN) 
    {
        estado = 0;
        f_controle();
    }
    else
    {
        estado++;
    }
    
    LATD = saida;
    
    atraso_ms(10);*/
    /*static uint8_t i = 0;
    if(i > 9)
        i = 0;
    display = conv7seg[i];
    i++;
    atraso_ms(500);*/
    
    disp1 = 1; disp2 = 0; disp3 = 0; disp4 = 0;
    display = conv7seg[1];
    atraso_ms(5);
    
    disp1 = 0; disp2 = 1; disp3 = 0; disp4 = 0;
    display = conv7seg[2];
    atraso_ms(5);
    
    disp1 = 0; disp2 = 0; disp3 = 1; disp4 = 0;
    display = conv7seg[3];
    atraso_ms(5);
    
    disp1 = 0; disp2 = 0; disp3 = 0; disp4 = 1;
    display = conv7seg[4];
    atraso_ms(5);
}

void f_int0(void)
{
    if(sel < 3)
        sel++;
    
    else
        sel = 0;
    
    LATB = sel<<6;
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
            saida = led1;
            //saida = 0b11110000;
            break;
        
        case 2:
            led2 = desloca_pra_direita(led2);
            led1 = desloca_pra_esquerda(led1);
            saida = led1;
            //saida = 0b01111000;
            break;
            
        case 3:
            led1 = desloca_pra_direita(led1);
            led2 = desloca_pra_esquerda(led2);
            saida = led1 | led2;
            //saida = 0b00111100;
            break;          
    }
}


uint8_t desloca_pra_direita(uint8_t dado)
{
    if(dado > 1)
        dado = dado >> 1;
    else
        dado = 128;
    
    return dado;
}

uint8_t desloca_pra_esquerda(uint8_t dado)
{
    if(dado < 128)
        dado = dado << 1;
    else
        dado = 1;
    
    return dado;
}