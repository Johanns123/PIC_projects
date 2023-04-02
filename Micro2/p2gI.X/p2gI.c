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

//Grupo I
//Johann Steffanno Amorim, Marisol Leitão dos Santos, Thalita Padua da Costa Ribeiro

#define _XTAL_FREQ 4000000

uint8_t buffer[10] ={0};
uint8_t segundo = 0, centesimo_de_segundo;
uint16_t Duty_cycle;
uint16_t porcentagem = 0, tempo_total = 0;

extern uint8_t mostra_centesimo_de_segundo, mostra_segundo, memoria_segundo, memoria_centesimo_de_segundo;
extern bool enable_time_config;

void f_pwm(uint16_t duty);
void loop(void);
void mostra_segundos_e_centesimos_de_segundos(void);
void show_display(void);

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
    show_display();
    mostra_segundos_e_centesimos_de_segundos();
    
    tempo_total = memoria_segundo*100 + memoria_centesimo_de_segundo;
    if(!enable_time_config)
        porcentagem = (Duty_cycle*100.0)/1023.0;
}

void mostra_segundos_e_centesimos_de_segundos(void)
{
    static uint16_t tempo = 0;
    tempo = AD_data[0];   //4 bits - de 0 a 15
    if(tempo > 1000)    tempo = 1000;
    segundo = tempo/100;
    centesimo_de_segundo = tempo%100;
    Duty_cycle = AD_data[1];
}

void show_display(void)
{
    if(enable_time_config)  //configurar o tempo
    {
        display1 = segundo / 10;
        display2 = segundo % 10;
        display3 = centesimo_de_segundo / 10;
        display4 = centesimo_de_segundo % 10;
    }
    
    else
    {
        display1 = mostra_segundo / 10;
        display2 = mostra_segundo % 10;
        display3 = mostra_centesimo_de_segundo / 10;
        display4 = mostra_centesimo_de_segundo % 10;
    }
}