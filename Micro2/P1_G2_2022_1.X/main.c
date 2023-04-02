/*
 * File:   mian.c
 * Author: Johann Steffanno e Guilherme Rocha
/*Alunos: Johann Steffanno Amorim, João Vitor Pessoa, Guilherme Santos da Rocha*/
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "configbits.h"
#include "ADC.h"
#include "Interrupts.h"
#include "Displays.h"
#include "IOs.h"

#define _XTAL_FREQ  4000000UL   //clock de 4MHz

uint8_t sequencia_1 = 0, sequencia_2 = 0, pausa_ou_continua = 1;

/*----------Prototipo das funcoes----------*/
void loop(void);
void atraso_ms(uint16_t tempo);
void contador_displays_01(void);
void contador_displays_02(void);
void read_keyb(void);
void Read_AD(void);
void counter_state(void);
void LED_State(void);

int main(void) {
    setup();
    while (1) loop();
    return 0;
}

void atraso_ms(uint16_t tempo) {
    while (tempo) {
        tempo--;
        __delay_ms(1);
    }
}

void f_timers(void) {
    static uint16_t counter0 = 1, counter1 = 1, counter2 = 1, counter3 = 1;

    if (counter0 < 5) //conto até 5 gerando 5 ms de tempo 
    {
        counter0++; //Incrementa o counter
    }
    else {
        mux_display(); //multiplexador de displays
        counter0 = 1;
    }

    if (counter1 < temporizador1)
    {
        counter1++; //Incrementa o counter
    }
    else {
        contador_displays_01();
        counter1 = 1;
    }
    
    if (counter2 < temporizador2)
    {
        counter2++; //Incrementa o counter
    }
    else {
        contador_displays_02();
        counter2 = 1;
    }
    
    if (counter3 < 10) //conto até 10 gerando 10 ms de tempo 
    {
        counter3++; //Incrementa o counter
    }
    
    else 
    {
        counter_state();
        LED_State();
        //read_keyb();
        Read_AD();
        counter3 = 1;
    }
}

void Read_AD(void)
{
    static uint8_t Valor_AD0 = 0;
    
    Valor_AD0 = AD_data[0] >> 6;
    
    switch(Valor_AD0)
    {
        case 0:
            sequencia_1 = 0; sequencia_2 = 0;
            break;
        
        case 1:
            sequencia_1 = 0; sequencia_2 = 1;
            break;
            
        case 2:
            sequencia_1 = 1; sequencia_2 = 0;
            break;
            
        case 3:
            sequencia_1 = 1; sequencia_2 = 1;
            break;
    }
}

void loop(void)
{
}


/*-----------Contador de 0 a 99*---------*/
void contador_displays_01(void) {

    if(!pausa_ou_continua)
    {
        if(!sequencia_1)
            cont1_crescente();

        else
            cont1_decrescente();
    }

}//end contador_displays


void contador_displays_02(void)
{
    if(!pausa_ou_continua)
    {
        if(!sequencia_2)
            cont2_crescente();

        else
            cont2_decrescente();
    }

}

void f_int0(void)
{
    static uint8_t estado = 0;
    
    switch(estado)
    {
        case 0:
            pausa_ou_continua = 0;
            estado = 1;
            break;
         
        case 1:
            pausa_ou_continua = 1;
            estado = 0;
            break;
    }
    
}//end f_int0

void f_int1(void) 
{
    static uint8_t sentido1 = 0;
    
    sentido1 ^= 1;  //XOR
    
    if(!sentido1)
    {
        temporizador1 = 300;
    }
    
    else
    {
        temporizador1 = 600;
    }
}//end f_int1

void f_int2(void) 
{
    static uint8_t sentido2 = 0;
    
    sentido2 ^= 1;
    
    if(!sentido2)
    {
        temporizador2 = 300;
    }
    
    else
    {
        temporizador2 = 600;
    }
}//end f_int2


void read_keyb(void)
{
    unsigned char dado, dado_RB3, dado_RB4;
    
    static unsigned char m_dado_RB3 = 1, m_dado_RB4 = 1;
    
    dado = PORTB;
    
    
    if (dado & 0b00001000)
    {  
        dado_RB3 = 1; 
        temporizador1 = 600;
    }
    else
    { 
        dado_RB3 = 0;
        temporizador1 = 300;
    }
    if (dado & 0b00010000)
    {
        dado_RB4 = 1;
        temporizador2 = 1000;
    }
    
    else
    {
        dado_RB4=0;
        temporizador2 = 500;
    }
    
    /*if (dado_RB3< m_dado_RB3)//li RB3 -> 0 - 300ms, 1-> 600ms
    {
        static uint8_t estado_tempo1 = 0;
        
        switch(estado_tempo1)
        {
            case 0:
                temporizador1 = 600;
                estado_tempo1 = 1;
                break;
                
            case 1:
                temporizador1 = 300;
                estado_tempo1 = 0;
                break;
        }
        
    }
    
    
    if (dado_RB4 < m_dado_RB4) //li RB4 -> 0 - 300ms, 1-> 600ms
    {
        static uint8_t estado_tempo2 = 0;
        
        switch(estado_tempo2)
        {
            case 0:
                temporizador2 = 1000;
                estado_tempo2 = 1;
                break;
                
            case 1:
                temporizador2 = 500;
                estado_tempo2 = 0;
                break;
        }
    }
    
    
    m_dado_RB3 = dado_RB3;
    m_dado_RB4 = dado_RB4;*/
}

void counter_state(void)
{
    if(!sequencia_1 && !sequencia_2)
    {
        display1 = 3;
    }
    
    else if(!sequencia_1 && sequencia_2)
    {
        display1 = 2;
    }
    
    else if(sequencia_1 && !sequencia_2)
    {
        display1 = 1;
    }
    
    else if(sequencia_1 && sequencia_2)
    {
        display1 = 0;
    }
}

void LED_State(void)
{
    if(pausa_ou_continua) //em pausa
    {
       LATB7 = 1; 
    }
    
    else    //contando
    {
        LATB7 = 0;
    }
    
    if(!sequencia_1)    //modo direto
    {
        LATB6 = 1;
    }
    
    else
    {
        LATB6 = 0;
    }
    
    if(!sequencia_2)    //modo direto
    {
        LATB5 = 1;
    }
    
    else
    {
        LATB5 = 0;
    }
    
    if(temporizador1 == 300) //300 ms de tempo
    {
        LATB4 = 1;
    }
    
    else  //600 ms de tempo
    {
        LATB4 = 0;
    }
    
    if(temporizador2 == 300) //300 ms de tempo
    {
        LATB3 = 1;
    }
    
    else //600 ms de tempo
    {
        LATB3 = 0;
    }
}