/*
 * File:   Interrupt.c
 * Author: johan
 *
 * Created on 22 de Novembro de 2022, 22:44
 */


#include <xc.h>
#include "lcd.h"
#include "Interrupt.h"
#include "Display.h"
#include <xc.h>
#include "Interrupt.h"
#include "ADC.h"
#include "teclado.h"

extern void read_keyboard (void);
extern void show_to_LCD(void);
extern void read_keyb(void);
extern void f_int0(void);
extern void f_int1(void);
extern void f_int2(void);
extern void Read_AD(void);

void __interrupt(high_priority) f_interrupt_high(void) //função chamada ao ocorrer uma interrupção
{
    if (TMR0IF) { //Testando se houve estouro no Timer0
        TMR0 = 0x06; //Reinicia o registrador Timer0

        f_timers(); //função do timer em que gerencio várias bases de tempo
        //Forma de gerar um processamento em "paralelo"

        TMR0IF = 0x00; //limpa a flag para a próxima interrupção
    }

    if (INT0IF && INT0IE) {
        //f_int0();
        INT0IF = 0;
    }

    if (INT1IF && INT1IE) {
        //f_int1();
        INT1IF = 0;
    }

    if (INT2IF && INT2IE) {
        //f_int2();
        INT2IF = 0;
    }
    
    if (ADIF && ADIE) {
        ADC_maq();
        ADIF = 0;
    }
}

void disable_interrupts(void) {
    IPEN = 0; //desabilito as interrupções por prioridade
    GIE = 0; //desabilito a chave de interrupção global
    PEIE = 0; //Desabilito a chave de interrupção dos periféricos
}

void enable_interrupts(void) {
    TMR0IE = 1; //Habilita a interrupção por estouro do Timer0
    ADIE = 1;   //Habilito interrupcao do AD
    PEIE = 1; //Habilito as interrupcoes de periferico
    GIE = 1; //Habilita as interrupções globais
}

void interrupt_init(void) 
{
    INTCON  = 0b10010000; //habilito a interrupção do INT0
    INTCON2 = 0b00000101; //habilito a interrupção para borda de descida, e outras interrupções com alta prioridade
    INTCON3 = 0b11011000; //habilito as interrupções externas e coloco-as em alta prioridade
    T0CON   = 0b11000001;
    //configura o TMR0 como contador de 8 bits
    //clock externo
    //associa o prescaler ao módulo timer0
    //Define o valor do prescaler no caso em 1:4 --> 1 no T0PS0 associado ao tmr0

    TMR0 = 0x06; //inicia a contagem em 6, tempo de 1ms
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
        show_to_LCD();
        counter1 = 1;
    }
    
    if (counter2 < temporizador2)
    {
        counter2++; //Incrementa o counter
    }
    else {
        counter2 = 1;
    }
    
    if (counter3 < 10) //conto até 10 gerando 10 ms de tempo 
    {
        counter3++; //Incrementa o counter
    }
    
    else 
    {
        read_keyboard();
        read_keyb();
        Read_AD();
        counter3 = 1;
    }
}