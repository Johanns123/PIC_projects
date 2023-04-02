#include <xc.h>
#include <stdio.h>
#include <stdbool.h>
#include "configbits.h"
#include "lcd.h"
#include "IO.h"
#include "Interrupt.h"
#include "Display.h"
#include "teclado.h"
#include "ADC.h"

//Johann Steffanno Amorim, Raquel Ferreira e Guilherme Rocha

#define _XTAL_FREQ 4000000

uint8_t buffer[10] ={0};
uint8_t minuto, segundo = 0;

extern uint8_t mostra_minuto, mostra_segundo;
extern bool active_line2, enable_time_config;

void f_pwm(uint16_t duty);
void loop(void);
void calcula_minuto_e_segundo(void);
void show_display(void);
extern void show_line2(); //Timer regressivo

int main(void)
{
    
   setup();
   while(1) loop(); 
   return 0;
}

void f_pwm(uint16_t duty)
{
    CCP1CON &= 0b11001111;  //zero DC1B1 me DC1B0
    CCP1CON |= ((duty & 0b11) << 4);  //atualizo os bits menos significativos
    CCPR1L = duty >> 2; //atualizo os bits mais significativos
}

void loop(void) 
{ 
    if(enable_time_config)
        show_display();
    
    if(active_line2)
    {
        show_line2(); //Timer regressivo
        active_line2 = 0;
    }
}


/* Converter leitura de 0-1023 para 0 a 15 minutos no AN0
 * Converter leitura de 0-1023 para 0 a 99 segundos no AN1
 * 
 * 
 * 1023 - 15
 * 0    -  0
 * 
 * minuto = 0.014*AD */
void calcula_minuto_e_segundo(void)
{
    minuto = AD_data[0] >> 6;   //4 bits - de 0 a 15
    segundo = (AD_data[1] >> 4);
    if(segundo > 59) segundo = 59;  
}

void show_display(void)
{
    display1 = minuto / 10;
    display2 = minuto % 10;
    display3 = segundo / 10;
    display4 = segundo % 10;
}