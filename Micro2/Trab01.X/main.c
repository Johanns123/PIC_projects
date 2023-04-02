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
#define TEMP_MIN    200
#define TEMP_MAX    800

//15 14 13...                   //0 1 2...
//contadorH  -> subtrai ate 15 , contadorL -> soma ate 15
//Contar a cada 500ms    contar a cada 900ms

uint8_t contador = 0;
uint8_t estado = 0;
uint8_t saida = 0;
uint8_t f_direcao, f_on_off;
uint8_t modo = 0;
uint8_t estado_de_operacao = 0;

void setup(void);
void loop(void);
void atraso_ms(uint16_t tempo);
void tecla(void);
void maq_1(void);
void apagar_leds(void);
void acende_descendo(void);
void acende_subindo(void);
void acende_subindo_e_descendo(void);

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
    
    ADCON1 = 0b00001111;
    TRISB  = 0b11111100;
    TRISD  = 0b00000000;
    LATD   = 0b00000000;
    LATB   = 0b00000000;
    
    f_direcao = 0;
    f_on_off = 0;
    
}

void loop(void) {
    
    tecla();
    
    maq_1(); 
    switch(estado_de_operacao)
    {
        case 0:
            apagar_leds();
            break;
           
        case 1:
            acende_descendo();
            break;
            
        case 2:
            acende_subindo();
            break;
            
        case 3:
            acende_subindo_e_descendo();
            break;
    }
    
    atraso_ms(10);
}

void maq_1(void)
{
    if (estado == 2) 
    {
        estado = 0;
    }
    else
    {
        estado++;
    }
}

void tecla(void)
{
    uint8_t dado , d_d, d_o;
    static uint8_t m_d=1, m_o=1; //m_d = memoria de direção e m_o = memoroia de on ou off
    
    dado = PORTB;
    if(dado & 0b00001000) 
        d_d=1;
    
    else 
        d_d=0;

    if(d_d < m_d)   //botao em borda de descida
    {
        
        switch (modo)
        {
            case 0:
                LATB = modo;
                estado_de_operacao = modo;
                modo = 1;
                break;
                
            case 1:
                LATB = modo;
                estado_de_operacao = modo;
                modo = 2;
                break;
            
            case 2:
                //led subindo
                estado_de_operacao = modo;
                LATB = modo;
                modo = 3;
                break;
            
            case 3:
                //sobe e desce
                estado_de_operacao = modo;
                LATB = modo;
                modo = 0;
                break;
        }
    }
    
    m_d = d_d;
    m_o = d_o;
}

void apagar_leds(void)
{
    LATD = 0b00000000;
}

void acende_descendo(void)
{
    static uint8_t contador = 1;
    static uint8_t estado = 0;
    
    if (estado == 30) 
    {
        switch (contador)
        {
        case 0:
            LATD = contador;
            contador = 1;
            break;
            
        case 1:
            LATD = contador;
            contador = 2;
            break;
            
        case 2:
            LATD = contador;
            contador = 4;
            break;
            
        case 4:
            LATD = contador;
            contador = 8;
            break;
            
        case 8:
            LATD = contador;
            contador = 16;
            break;
            
        case 16:
            LATD = contador;
            contador = 32;
            break;
            
        case 32:
            LATD = contador;
            contador = 64;
            break;
            
        case 64:
            LATD = contador;
            contador = 128;
            break;
            
        case 128:
            LATD = contador;
            contador = 1;
            break;
       
        }
        estado = 0;
    }
    else
    {
        estado++;
    }
    
}
void acende_subindo(void)
{
    static uint8_t contador = 128;
    static uint8_t estado = 0;
    
    if (estado == 30) 
    {
        switch (contador)
        {
        case 0:
            LATD = contador;
            contador = 128;
            break;
            
        case 1:
            LATD = contador;
            contador = 128;
            break;
            
        case 2:
            LATD = contador;
            contador = 1;
            break;
            
        case 4:
            LATD = contador;
            contador = 2;
            break;
            
        case 8:
            LATD = contador;
            contador = 4;
            break;
            
        case 16:
            LATD = contador;
            contador = 8;
            break;
            
        case 32:
            LATD = contador;
            contador = 16;
            break;
            
        case 64:
            LATD = contador;
            contador = 32;
            break;
            
        case 128:
            LATD = contador;
            contador = 64;
            break;
       
        }
        estado = 0;
    }
    else
    {
        estado++;
    }
}
void acende_subindo_e_descendo(void)
{
    static uint8_t contador = 0;
    static uint8_t estado = 0;
    static uint8_t sentido = 0, primeira_vez = 0;
    
    
    if (estado == 30) 
    {
        switch (contador)
        {
        case 0:
            LATD = contador;
            contador = 0b10000001;
            break;
            
        case 0b10000001:
            LATD = contador;
            contador = 0b01000010;
            break;
            
        case 0b01000010:
            LATD = contador;
            if(sentido || !primeira_vez)
            {
                contador = 0b00100100;
                primeira_vez = 1;
                sentido = 0;
            }
            
            else
            {   
                contador = 0b10000001;
                sentido = 1;
            }
            break;
            
        case 0b00100100:
            LATD = contador;
            if(!sentido)
            {
                contador = 0b00011000;
                sentido = 1;
            }
            
            else
            {
                contador = 0b01000010;
                sentido = 0;
            }
            break;
            
        case 0b00011000:
            LATD = contador;
            contador = 0b00100100;
            break;
            
       
        }
        estado = 0;
    }
    else
    {
        estado++;
    }
    
}