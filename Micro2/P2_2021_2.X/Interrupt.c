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
#include "ADC.h"
#include "teclado.h"
#include <stdbool.h>
#include <stdio.h>

void read_keyb(void);
void f_int0(void);
void f_int1(void);
void f_int2(void);
void f_int3(void);
void f_int4(void);
void f_int5(void);
void clock_regressive(void);
void show_line1(void);
void show_line2(void);
extern void calcula_minuto_e_segundo(void);
extern void show_display(void);
extern void f_pwm(uint16_t duty);

uint8_t buffer[10];
extern uint8_t minuto, segundo;
bool start_counter = 0, enable_time_config = 1, active_line2 = 0;
uint8_t mostra_minuto = 0, mostra_segundo = 0, memoria_minuto = 0, memoria_segundo = 1;
uint16_t temporizacao = 1000;
uint8_t dezena_minuto = 0, unidade_minuto = 0, dezena_segundo = 0, unidade_segundo = 0;


void __interrupt(high_priority) f_interrupt_high(void) //função chamada ao ocorrer uma interrupção
{
    if (TMR0IF && TMR0IE) { //Testando se houve estouro no Timer0
        TMR0 += 0x06; //Reinicia o registrador Timer0

        f_timers(); //função do timer em que gerencio várias bases de tempo
        //Forma de gerar um processamento em "paralelo"

        TMR0IF = 0x00; //limpa a flag para a próxima interrupção
    }

    if (INT0IF && INT0IE) {
        INT0IF = 0;
    }

    if (INT1IF && INT1IE) {
        INT1IF = 0;
    }

    if (INT2IF && INT2IE) {
        INT2IF = 0;
    }
    
    if (ADIF && ADIE && PEIE) {
        ADIF = 0;
    }
    
    if (TMR2IF && TMR2IE  && PEIE) 
    {
        //RE0 ^= 1;
        TMR2IF = 0;
    }
    
    /*if (TMR1IF && TMR1IE  && PEIE) 
    {
        TMR1 = 65286; 
        RE0 ^= 1;
        //read_RPM();
        TMR1IF = 0;
    }*/
}

void disable_interrupts(void) {
    IPEN = 0; //desabilito as interrupções por prioridade
    GIE = 0; //desabilito a chave de interrupção global
    PEIE = 0; //Desabilito a chave de interrupção dos periféricos
}

void enable_interrupts(void) {
    TMR0IE = 1; //Habilita a interrupção por estouro do Timer0
    ADIE = 1;   //Habilito interrupcao do AD
    PEIE = 1; //Habilito as interrupcoes de periferico (bug do simulador) - desabilitado para mostrar no display
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
    //Define o valor do prescaler no caso em 1:1 --> 1 no T0PS0 associado ao tmr0

    TMR0 = 0x06; //inicia a contagem em 6, tempo de 1 ms
}
void f_timers(void) {
    static uint16_t counter1 = 1, counter2 = 1, counter3 = 1, counter4 = 1, counter5 = 1;
           
    RE0 ^= 1;
    
    ADC_maq();

    if (counter1 < 5)
    {
        counter1++; //Incrementa o counter
    }
    
    else
    {
        mux_display(); //multiplexador de displays
        counter1 = 1;
    }
    
    
    if (counter2 < 10) //conto até 10 gerando 10 ms de tempo 
    {
        counter2++; //Incrementa o counter
    }
    
    else 
    {
        read_keyb();
        calcula_minuto_e_segundo();
        counter2 = 1;
    }
    
    if (counter3 < temporizacao)
    {
        counter3++; //Incrementa o counter
    }
    
    else
    {
        if(start_counter)
            clock_regressive(); //Timer regressivo
        counter3 = 1;
    }
    
    if (counter4 < 100) //a cada 100 ms
    {
        counter4++; //Incrementa o counter
    }
    
    else
    {
        active_line2 = 1;
        counter4 = 1;
    }
    
}

void read_keyb(void)
{
    uint8_t dado, dado_RB0, dado_RB1, dado_RB2, dado_RB3, dado_RB4, dado_RB5;
    
    static uint8_t m_dado_RB0 = 1, m_dado_RB1 = 1, m_dado_RB2 = 1, m_dado_RB3 = 1, m_dado_RB4 = 1, m_dado_RB5 = 1;
    
    dado = PORTB;
    
    if (dado & 0b00000001)
    {  
        dado_RB0 = 1; 
    }
    else
    { 
        dado_RB0 = 0;
    }
    
    if (dado & 0b00000010)
    {  
        dado_RB1 = 1; 
    }
    else
    { 
        dado_RB1 = 0;
    }
    
    if (dado & 0b00000100)
    {  
        dado_RB2 = 1; 
    }
    else
    { 
        dado_RB2 = 0;
    }
    
    
    if (dado & 0b00001000)
    {  
        dado_RB3 = 1; 
    }
    else
    { 
        dado_RB3 = 0;
    }
    
    if (dado & 0b00010000)
    {
        dado_RB4 = 1;
    }
    
    else
    {
        dado_RB4 = 0;
    }
    
    if (dado & 0b00100000)
    {
        dado_RB5 = 1;
    }
    
    else
    {
        dado_RB5=0;
    }
    
    if (dado_RB0< m_dado_RB0)//li RB3 -> 0 - 300ms, 1-> 600ms
    {
        f_int0();
    }

    if (dado_RB1 < m_dado_RB1) //li RB4 -> 0 - 300ms, 1-> 600ms
    {
        f_int1();
    }
    
    if (dado_RB2 < m_dado_RB2) //li RB4 -> 0 - 300ms, 1-> 600ms
    {
        f_int2();
    }
    
    if (dado_RB3< m_dado_RB3)//li RB3 -> 0 - 300ms, 1-> 600ms
    {
        f_int3();
    }

    if (dado_RB4 < m_dado_RB4) //li RB4 -> 0 - 300ms, 1-> 600ms
    {
        f_int4();
    }
    
    if (dado_RB5 < m_dado_RB5) //li RB4 -> 0 - 300ms, 1-> 600ms
    {
        f_int5();
    }
    
    m_dado_RB0 = dado_RB0;
    m_dado_RB1 = dado_RB1;
    m_dado_RB2 = dado_RB2;
    m_dado_RB3 = dado_RB3;
    m_dado_RB4 = dado_RB4;
    m_dado_RB5 = dado_RB5;
}

void f_int0(void)
{
    mostra_minuto = mostra_segundo = 0;
    enable_time_config = 0;
    f_pwm(1023);
}

void f_int1(void)
{
    enable_time_config = 0;
    start_counter = 1;
    memoria_minuto = mostra_minuto = minuto;
    memoria_segundo = mostra_segundo = segundo;
    show_line1();
}

void f_int2(void)
{
    start_counter = 0;
    enable_time_config = 1;
    f_pwm(0);
}

void f_int3(void)
{
    temporizacao = 1000;
    show_line1();
}

void f_int4(void)
{
    temporizacao = 100;
    show_line1();
}

void f_int5(void)
{
    temporizacao = 10;
    show_line1();
}

void clock_regressive(void)
{
    
    if(mostra_segundo < 1 && mostra_minuto)
    {
        mostra_segundo = 59;
        mostra_minuto--;
        return;
    }
    
    mostra_segundo--;
    
    if(mostra_minuto < 1 && mostra_segundo < 1)
    {
        mostra_minuto = mostra_segundo = 0;
        start_counter = 0;  //encerrou a contagem
        f_pwm(1023);
    }
}


void show_line1(void)
{
    lcd_cmd(L_L1);
    sprintf(buffer, "%2.2d:%2.2d tempo:%4d", memoria_minuto, memoria_segundo, temporizacao);
    lcd_str(buffer);
}

void show_line2(void)
{      
    static uint8_t old_latd = 0;
    old_latd = LATD;
    lcd_cmd(L_L2);
    sprintf(buffer, "     %2.2d:%2.2d", mostra_minuto, mostra_segundo);
    lcd_str(buffer);
    LATD = old_latd;
}