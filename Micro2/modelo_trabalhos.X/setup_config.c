/*
 * File:   setup_config.c
 * Author: Jefferson Henrique
 *
 * Created on 2 de Novembro de 2022, 10:40
 */

#include "main.h"
#include "setup_config.h"
#include "disp_7seg.h"        // ações do display_7_segmentos
#include "adc.h"
#include "pwm.h"
#include "lcd.h"

void disable_interrupts(void)
{
    IPEN = 0;       //desabilito as interrupções por prioridade
    GIE  = 0;        //desabilito a chave de interrupção global
    PEIE = 0;       //Desabilito a chave de interrupção dos periféricos
}

void setup(void) 
{
    setup_hardware();
    setup_software();
}

void setup_hardware(void)
{
    disable_interrupts();
    setup_io();             //defino os dispositivos de entrada e saída
    setup_interrupt();
    setup_timer0();
    setup_ad();
    setup_pwm();
    enable_interrupts();
}

void setup_software(void)
{
    /*Configuracoes de tempo*/
    timer_0 =  1;
    timer_1 = 10;
    timer_2 = 20;
    timer_3 = 40;
    /*Configuracoes inicializacao dos displays de 7segmentos*/
    display1 = 1; 
    display2 = 2; 
    display3 = 3;
    display4 = 4;
    /*Inicializacao do AD*/
    start_AD_conversion();
    /*Selecao de como usar o PORTD*/
    set_portd = DISPLAYS; // -> 0 = DISPLAYS || 1 = ADC
    
    lcd_init();
    /*Valor inicial de PWM*/
    pwm_dutycycle(800);
    
    /*Apaga o LCD para iniciar*/
    lcd_cmd(L_CLR);
}

void setup_io(void)
{
    ADCON1  = 0b00001111;   //pinos com AD desconfigurado
    //TRISA   = 0b11000011;   //RA5 -> RA2 como saida 
    TRISA   = 0b11000011;
    TRISB   = 0b00111111;
    TRISC   = 0b11011011;
    TRISD   = 0b00000000;   //tudo como saida
    TRISE   = 0b11111001;
    LATA    = 0b00000000;   //Inicio em nível baixo
    LATD    = 0b00000000;   //Inicio em nível baixo
    LATB    = 0b00000000;   //Inicio em nível baixo
    LATC    = 0b00100000;
}

void setup_ad(void)
{
    ADCON0 = 0b00000001; //perguntar canal
    ADCON1 = 0b00001101; //A0 e A1 como leitura
    //ADCON2 = 0b00010100;
    ADCON2 = 0b10010100;
    
    //ADRESH = 0; // parte alta
    //ADRESL = 0; // parte baixa
}

void setup_interrupt(void)
{
    INTCON  = 0b00110000;    // habilito a interrupção do INT0
    INTCON2 = 0b00000101;    // habilito a interrupção para borda de descida, e outras interrupções com alta prioridade
    INTCON3 = 0b11011000;    // habilito as interrupções externas e coloco-as em alta prioridade

}

void setup_timer0(void)
{
    T0CON   = 0b11010001;
    //configura o TMR0 como contador de 8 bits
    //clock externo
    //associa o prescaler ao módulo timer0
    //Define o valor do prescaler no caso em 1:4 --> 1 no T0PS0 associado ao tmr0
    
    TMR0L   = 6;             // inicia a contagem em 6, tempo de 1ms
}

void setup_pwm(void)
{
    //timer2
    T2CON = 0b00000101;
    PR2 = 255;
    
    //PWM 1
    CCP1CON = 0b00001100;
}

void enable_interrupts(void)
{
    TMR0IE = 1;             //Habilita a interrupção por estouro do Timer0
    GIE    = 1;             //Habilita as interrupções globais
}

void setup_bottons(bool botao3, bool botao4, bool botao5)
{
    switch(botao3)
    {
        case PRESSED:
            if(!leitura_botao_pressed(3)) leitura_b3 = 1; else leitura_b3 = 0;
            break;
        case CLICK:
            if(leitura_botao_click(3)) leitura_b3 = !leitura_b3;
            break;
    }
    switch(botao4)
    {
        case PRESSED:
            if(!leitura_botao_pressed(4)) leitura_b4 = 1; else leitura_b4 = 0;
            break;
        case CLICK:
            if(leitura_botao_click(4)) leitura_b4 = !leitura_b4;
            break;
    }
    switch(botao5)
    {
        case PRESSED:
            if(!leitura_botao_pressed(5)) leitura_b5 = 1; else leitura_b5 = 0;
            break;
        case CLICK:
            if(leitura_botao_click(5)) leitura_b5 = !leitura_b5;
            break;
    }
}

    
    
    
    
    
    
    