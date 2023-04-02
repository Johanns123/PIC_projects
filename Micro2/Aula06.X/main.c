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

#define _XTAL_FREQ  4000000UL   //clock de 4MHz

#define disp1 LATA2
#define disp2 LATA3
#define disp3 LATA4
#define disp4 LATA5

#define display LATD

uint8_t dezena_disp_par01 = 0, dezena_disp_par02 = 0, unidade_disp_par01 = 0, unidade_disp_par02 = 0;

uint8_t conv7seg[17] =      //conversor pro display de 7 segmentos 
{
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

void setup(void);
void setup_io(void);
void interrupt_init(void);
void loop(void);
void atraso_ms(uint16_t tempo);
void f_int0(void);
void f_int1(void);
void f_int2(void);
void f_controle(void);
void f_timers(void);
uint8_t desloca_pra_direita(uint8_t dado);
uint8_t desloca_pra_esquerda(uint8_t dado);
void mux_display(void);
void contador_displays(void);
void disable_interrupts(void);
void enable_interrupts(void);

void __interrupt (high_priority) f_interrupt_high (void) //função chamada ao ocorrer uma interrupção
{
    if(TMR0IF && TMR0IE)
    {          //Testando se houve estouro no Timer0
        TMR0 += 0x06;                //Reinicia o registrador Timer0
        
        f_timers();                 //função do timer em que gerencio várias bases de tempo
                                    //Forma de gerar um processamento em "paralelo"
        
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
    static uint16_t counter0 = 1, counter1 = 1;
    
    if(counter0 < 5)       //conto até 5 gerando 5 ms de tempo 
    {  
        counter0++;                  //Incrementa o counter
    }
        
    else 
    {
        mux_display();  //multiplexador de displays
        counter0 = 1;
    }
    
    
    if(counter1 < 500) //conto até 500 gerando 500 ms de tempo
    {  
        counter1++;                  //Incrementa o counter
    }
        
    else 
    {
        contador_displays();
        counter1 = 1;
    }
}

void setup(void) 
{
    disable_interrupts();
    setup_io();             //defino os dispositivos de entrada e saída
    interrupt_init();
    disp1 = 1, disp2 = 1, disp3 = 1, disp4 = 1;
    enable_interrupts();
}

void disable_interrupts(void)
{
    IPEN = 0;       //desabilito as interrupções por prioridade
    GIE = 0;        //desabilito a chave de interrupção global
    PEIE = 0;       //Desabilito a chave de interrupção dos periféricos
}

void enable_interrupts(void)
{
    TMR0IE = 1;          //Habilita a interrupção por estouro do Timer0
    GIE = 1;            //Habilita as interrupções globais
}

void setup_io(void)
{
    ADCON1  = 0b00001111;   //pinos com AD desconfigurado
    TRISA   = 0b11000011;   //RA5 -> RA2 como saida 
    TRISB   = 0b00000011;   //tudo como entrada
    TRISD   = 0b00000000;   //tudo como saida
    LATA    = 0b00000000;   //Inicio em nível baixo
    LATD    = 0b00000000;   //Inicio em nível baixo
    LATB    = 0b00000000;   //Inicio em nível baixo
}

void interrupt_init(void)
{
    INTCON  = 0b10010000;    //habilito a interrupção do INT0
    INTCON2 = 0b00000101;    //habilito a interrupção para borda de descida, e outras interrupções com alta prioridade
    INTCON3 = 0b11011000;    //habilito as interrupções externas e coloco-as em alta prioridade
    T0CON = 0b11010001;
    //configura o TMR0 como contador de 8 bits
    //clock externo
    //associa o prescaler ao módulo timer0
    //Define o valor do prescaler no caso em 1:4 --> 1 no T0PS0 associado ao tmr0
    
    TMR0 = 0x06;                        //inicia a contagem em 6, tempo de 1ms
}

void loop(void) 
{   
    /*static uint8_t num = 123;
    
    dezena_disp_par01 = 16;
    unidade_disp_par01 = num >> 4;
    dezena_disp_par02 = num & 0b00001111;
    unidade_disp_par02 = 16;*/

}

/*Faço a multiplexação dos displays para dar um efeito visual de paralelismo*/
void mux_display()
{
    static uint8_t estado = 0;
    
    switch(estado)
    {
        case 0:     //acendo o display 1 e apago os outros
            disp4 = 0;
            disp1 = 1;
            display = conv7seg[dezena_disp_par01];      //passo para os segmentos do display o número a ser mostrado
            estado = 1;
            break;
        
        case 1: //acendo o display 2 e apago os outros
            disp1 = 0;
            disp2 = 1;
            display = conv7seg[unidade_disp_par01];
            estado = 2;
            break;
        
        case 2: //acendo o display 3 e apago os outros
            disp2 = 0;
            disp3 = 1;
            display = conv7seg[dezena_disp_par02];
            estado = 3;
            break;
        
        case 3: //acendo o display 4 e apago os outros
            disp3 = 0;
            disp4 = 1;
            display = conv7seg[unidade_disp_par02];
            estado = 0;
            break;
            
        default:
            disp1 = 0;
            disp2 = 0;
            disp3 = 0;
            disp4 = 0;
            estado = 0;
            break;
    }
}    

/*-----------Contador de 0 a 99*---------*/
void contador_displays(void)
{
    unidade_disp_par01++;
    unidade_disp_par02--;
    //unidade_disp_par02++;
    
    
    if(unidade_disp_par01 > 9)
    {
        dezena_disp_par01++;
        unidade_disp_par01 = 0;
    }
    
    if(dezena_disp_par01 > 9)
    {
        dezena_disp_par01 = 0;
    }
    
    /*if(unidade_disp_par02 > 9)
    {
        dezena_disp_par02++;
        unidade_disp_par02 = 0;
    }
    
    if(dezena_disp_par02 > 9)
    {
        dezena_disp_par02 = 0;
    }*/
    
    if(unidade_disp_par02 < 1 || unidade_disp_par02 == 255)
    {
        dezena_disp_par02--;
        unidade_disp_par02 = 9;
    }
    
    if(dezena_disp_par02 < 0 || dezena_disp_par02 == 255)
    {
        dezena_disp_par02 = 9;
    }
}//end contador_displays

void f_int0(void)
{   
}//end f_int0

void f_int1(void)
{
}//end f_int1

void f_int2(void)
{

}//end f_int2