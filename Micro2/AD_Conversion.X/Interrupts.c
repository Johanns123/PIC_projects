/*
 * File:   Interrupts.c
 * Author: johanns123
 *
 * Created on 1 de Novembro de 2022, 23:39
 */


#include <xc.h>
#include "Interrupts.h"
#include "ADC.h"

void __interrupt(high_priority) f_interrupt_high(void) //função chamada ao ocorrer uma interrupção
{
    if (TMR0IF) { //Testando se houve estouro no Timer0
        TMR0 = 0x06; //Reinicia o registrador Timer0

        f_timers(); //função do timer em que gerencio várias bases de tempo
        //Forma de gerar um processamento em "paralelo"

        TMR0IF = 0x00; //limpa a flag para a próxima interrupção
    }

    if (INT0IF && INT0IE) {
        f_int0();
        INT0IF = 0;
    }

    if (INT1IF && INT1IE) {
        f_int1();
        INT1IF = 0;
    }

    if (INT2IF && INT2IE) {
        f_int2();
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

void interrupt_init(void) {
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