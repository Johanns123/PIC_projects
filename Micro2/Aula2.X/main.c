/*
 * File:   mian.c
 * Author: Johann Steffanno e Guilherme Rocha
 *
 * Created on 21 de Setembro de 2022, 19:15
 */

#include <xc.h>
#include <stdint.h>
#include "configbits.h"

#define _XTAL_FREQ 4000000UL
#define TEMP_MIN    500
#define TEMP_MAX    900

//15 14 13...                   //0 1 2...
//contadorH  -> subtrai ate 15 , contadorL -> soma ate 15
//Contar a cada 500ms    contar a cada 900ms

uint8_t contadorH = 15, contadorL = 0; 
uint16_t estadoH = 0, estadoL = 0;

void setup(void);
void loop(void);

int main(void)
{
    setup();
    while(1)    loop();
    return 0;
}

void setup(void)
{
    ADCON1  = 0x0f;  //desabilita as portas analógicas
    TRISD   = 0x00;   //Tudo como saida
    LATD    = 0x00;    
}

void loop(void)
{
   
    if(estadoH < TEMP_MIN)
    {
        estadoH++;
    }
    
    else
    {
        contadorH--;
        LATD = (contadorH<<4) | (contadorL); 
        estadoH = 1;
    }
    
    if(estadoL < TEMP_MAX)
    {
        estadoL++;
    }
    
    else
    {
        contadorL++;
        LATD = (contadorH<<4) | (contadorL); 
        estadoL = 1;
    }
    
    if(contadorH < 0)
        contadorH = 15;
    
    if(contadorL > 15)
        contadorL = 0; 
    __delay_ms(1);

}
