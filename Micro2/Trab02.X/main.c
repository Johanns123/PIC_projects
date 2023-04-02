/*
 * File:   mian.c
 * Author: Johann Steffanno e Guilherme Rocha
 *
 * Created on 27 de Setembro de 2022, 19:20
 */


/*
TRISx - define se a porta ou o pino desejado é entrada ou saída
LATx  - coloca o pino em nível lógico alto (ex. 5V) ou baixo - usado somente em pinos de saída 
PORTx - utilizado para ler a informação do pino, se está em nível lógico alto ou baixo
normalmente utilizado para leitura de pinos de entrada 
obs.: x significa qualquer letra referente aa alguma porta, ex.: A, B, C, D... 
 * 
 * 
 * 
 * Timer de 2 bits
 * 00
 * 01
 * 10
 * 11
 * 111 - estouro do timer
 * /\
 * |
 * Carry
 * Gera a interrupção
 * 00
 * 
 * 6ms
 * 
 * Valor inicial da contagem = n° total de bits do timer - (tempo desejado(em segundos)*FMAQ/prescaler)
 * FMAQ = FOSC/4 = 1MHz
 * Valor inicial = 256 - (0,006*1000000/32) = 68,5 = 68
 */


/*Alunos: Johann Steffanno Amorim, João Vitor Pessoa, Guilherme Santos da Rocha*/
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

uint8_t display1 = 0, display2 = 0, display3 = 0, display4 = 0;
uint8_t estado_sequencia01 = 1, estado_sequencia02 = 0;
uint8_t conv7seg[17] = //conversor pro display de 7 segmentos 
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
uint8_t sequencia_1 = 0, sequencia_2 = 0, pausa_ou_continua = 0;
uint16_t temporizador1 = 0, temporizador2 = 0;
/*----------Prototipo das funcoes----------*/
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
void contador_displays_01(void);
void contador_displays_02(void);
void disable_interrupts(void);
void enable_interrupts(void);
void cont1_crescente(void);
void cont1_decrescente(void);
void cont2_crescente(void);
void cont2_decrescente(void);
void read_keyb(void);

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
}

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
    else {
        read_keyb(); //multiplexador de displays
        counter3 = 1;
    }
}

void setup(void) {
    disable_interrupts();
    setup_io(); //defino os dispositivos de entrada e saída
    interrupt_init();
    disp1 = 1, disp2 = 1, disp3 = 1, disp4 = 1;
    temporizador1 = 300;
    temporizador2 = 500;
    enable_interrupts();
}

void disable_interrupts(void) {
    IPEN = 0; //desabilito as interrupções por prioridade
    GIE = 0; //desabilito a chave de interrupção global
    PEIE = 0; //Desabilito a chave de interrupção dos periféricos
}

void enable_interrupts(void) {
    TMR0IE = 1; //Habilita a interrupção por estouro do Timer0
    GIE = 1; //Habilita as interrupções globais
}

void setup_io(void) {
    ADCON1 = 0b00001111; //pinos com AD desconfigurado
    TRISA = 0b11000011; //RA5 -> RA2 como saida 
    TRISB = 0b00011111; //RB0 -> RB2 como entrada
    TRISD = 0b00000000; //tudo como saida
    LATA = 0b00000000; //Inicio em nível baixo
    LATD = 0b00000000; //Inicio em nível baixo
    LATB = 0b00000000; //Inicio em nível baixo
}

void interrupt_init(void) {
    INTCON = 0b10010000; //habilito a interrupção do INT0
    INTCON2 = 0b00000101; //habilito a interrupção para borda de descida, e outras interrupções com alta prioridade
    INTCON3 = 0b11011000; //habilito as interrupções externas e coloco-as em alta prioridade
    T0CON = 0b11000001;
    //configura o TMR0 como contador de 8 bits
    //clock externo
    //associa o prescaler ao módulo timer0
    //Define o valor do prescaler no caso em 1:4 --> 1 no T0PS0 associado ao tmr0

    TMR0 = 0x06; //inicia a contagem em 6, tempo de 1ms
}

void loop(void) {
}

/*Faço a multiplexação dos displays para dar um efeito visual de paralelismo*/
void mux_display() {
    static uint8_t estado = 0;

    switch (estado) {
        case 0: //acendo o display 1 e apago os outros
            disp1 = 1;
            disp4 = 0;
            display = conv7seg[display1]; //passo para os segmentos do display o número a ser mostrado
            estado = 1;
            break;

        case 1: //acendo o display 2 e apago os outros
            disp1 = 0;
            disp2 = 1;
            display = conv7seg[display2];
            estado = 2;
            break;

        case 2: //acendo o display 3 e apago os outros
            disp2 = 0;
            disp3 = 1;
            display = conv7seg[display3];
            estado = 3;
            break;

        case 3: //acendo o display 4 e apago os outros
            disp3 = 0;
            disp4 = 1;
            display = conv7seg[display4];
            estado = 0;
            break;
    }
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

void cont1_crescente(void)
{
     switch (estado_sequencia01) {
        case 1:
            display1 = 0;
            display2 = 1;
            estado_sequencia01 = 2;
            break;

        case 2:
            display1 = 0;
            display2 = 2;
            estado_sequencia01 = 4;
            break;

        case 4:
            display1 = 0;
            display2 = 4;
            estado_sequencia01 = 5;
            break;
            
        case 5:
            display1 = 0;
            display2 = 5;
            estado_sequencia01 = 7;
            break;

        case 7:
            display1 = 0;
            display2 = 7;
            estado_sequencia01 = 8;
            break;
        
        case 8:
            display1 = 0;
            display2 = 8;
            estado_sequencia01 = 10;
            break;
        
        case 10:
            display1 = 1;
            display2 = 0;
            estado_sequencia01 = 11;
            break;
        
        case 11:
            display1 = 1;
            display2 = 1;
            estado_sequencia01 = 13;
            break;
        
        case 13:
            display1 = 1;
            display2 = 3;
            estado_sequencia01 = 14;
            break;
        
        case 14:
            display1 = 1;
            display2 = 4;
            estado_sequencia01 = 16;
            break;
        
        case 16:
            display1 = 1;
            display2 = 6;
            estado_sequencia01 = 17;
            break;
            
         case 17:
            display1 = 1;
            display2 = 7;
            estado_sequencia01 = 19;
            break;
            
         case 19:
            display1 = 1;
            display2 = 9;
            estado_sequencia01 = 20;
            break;
        
        case 20:
            display1 = 2;
            display2 = 0;
            estado_sequencia01 = 1;
            break;
            
        default:
            display1 = 0;
            display2 = 0;
            estado_sequencia01 = 0;
            break;
    }
}

void cont1_decrescente(void)
{
     switch (estado_sequencia01) {

        case 1:
            display1 = 0;
            display2 = 1;
            estado_sequencia01 = 20;
            break;

        case 2:
            display1 = 0;
            display2 = 2;
            estado_sequencia01 = 1;
            break;

        case 4:
            display1 = 0;
            display2 = 4;
            estado_sequencia01 = 2;
            break;
            
        case 5:
            display1 = 0;
            display2 = 5;
            estado_sequencia01 = 4;
            break;

        case 7:
            display1 = 0;
            display2 = 7;
            estado_sequencia01 = 5;
            break;
        
        case 8:
            display1 = 0;
            display2 = 8;
            estado_sequencia01 = 7;
            break;
        
        case 10:
            display1 = 1;
            display2 = 0;
            estado_sequencia01 = 8;
            break;
        
        case 11:
            display1 = 1;
            display2 = 1;
            estado_sequencia01 = 10;
            break;
        
        case 13:
            display1 = 1;
            display2 = 3;
            estado_sequencia01 = 11;
            break;
        
        case 14:
            display1 = 1;
            display2 = 4;
            estado_sequencia01 = 13;
            break;
        
        case 16:
            display1 = 1;
            display2 = 6;
            estado_sequencia01 = 14;
            break;
            
         case 17:
            display1 = 1;
            display2 = 7;
            estado_sequencia01 = 16;
            break;
            
         case 19:
            display1 = 1;
            display2 = 9;
            estado_sequencia01 = 17;
            break;
        
        case 20:
            display1 = 2;
            display2 = 0;
            estado_sequencia01 = 19;
            break;
            
        default:
            display1 = 0;
            display2 = 0;
            estado_sequencia01 = 20;
            break;
    }
}

void cont2_crescente(void)
{
    switch (estado_sequencia02)
    {
        case 0:
            display3 = 0;
            display4 = 0;       
            estado_sequencia02 = 1;
            break;
            
        case 1:
            display3 = 0;
            display4 = 1;       
            estado_sequencia02 = 4;
            break;

        case 4:
            display3 = 0;
            display4 = 4;
            estado_sequencia02 = 6;
            break;

        case 6:
            display3 = 0;
            display4 = 6;
            estado_sequencia02 = 8;
            break;

        case 8:
            display3 = 0;
            display4 = 8;
            estado_sequencia02 = 9;
            break;
            
        case 9:
            display3 = 0;
            display4 = 9;
            estado_sequencia02 = 10;
            break;

        case 10:
            display3 = 0;
            display4 = 10;
            estado_sequencia02 = 12;
            break;
        
        case 12:
            display3 = 0;
            display4 = 12;
            estado_sequencia02 = 14;
            break;
        
        case 14:
            display3 = 0;
            display4 = 14;
            estado_sequencia02 = 15;
            break;
        
        case 15:
            display3 = 0;
            display4 = 15;
            estado_sequencia02 = 16;
            break;
        
        case 16:
            display3 = 1;
            display4 = 0;
            estado_sequencia02 = 18;
            break;
        
        case 18:
            display3 = 1;
            display4 = 2;
            estado_sequencia02 = 20;
            break;
        
        case 20:
            display3 = 1;
            display4 = 4;
            estado_sequencia02 = 0;
            break;
            
        default:
            display3 = 0;
            display4 = 0;
            estado_sequencia02 = 0;
            break;
    }
}

void cont2_decrescente(void)
{
    switch (estado_sequencia02)
    {
        case 0:
            display3 = 0;
            display4 = 0;       
            estado_sequencia02 = 20;
            break;
            
        case 1:
            display3 = 0;
            display4 = 1;       
            estado_sequencia02 = 0;
            break;
        
        case 4:
            display3 = 0;
            display4 = 4;
            estado_sequencia02 = 1;
            break;

        case 6:
            display3 = 0;
            display4 = 6;
            estado_sequencia02 = 4;
            break;

        case 8:
            display3 = 0;
            display4 = 8;
            estado_sequencia02 = 6;
            break;
            
        case 9:
            display3 = 0;
            display4 = 9;
            estado_sequencia02 = 8;
            break;

        case 10:
            display3 = 0;
            display4 = 10;
            estado_sequencia02 = 9;
            break;
        
        case 12:
            display3 = 0;
            display4 = 12;
            estado_sequencia02 = 10;
            break;
        
        case 14:
            display3 = 0;
            display4 = 14;
            estado_sequencia02 = 12;
            break;
        
        case 15:
            display3 = 0;
            display4 = 15;
            estado_sequencia02 = 14;
            break;
        
        case 16:
            display3 = 1;
            display4 = 0;
            estado_sequencia02 = 15;
            break;
        
        case 18:
            display3 = 1;
            display4 = 2;
            estado_sequencia02 = 16;
            break;
        
        case 20:
            display3 = 1;
            display4 = 4;
            estado_sequencia02 = 18;
            break;
            
        default:
            display3 = 0;
            display4 = 0;
            estado_sequencia02 = 20;
            break;
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
    static uint8_t estado = 0;
    
    switch(estado)
    {
        case 0:
            sequencia_1 = 0;
            estado = 1;
            break;
         
        case 1:
            sequencia_1 = 1;
            estado = 0;
            break;
    }
}//end f_int1

void f_int2(void) 
{
    static uint8_t estado = 0;
    
    switch(estado)
    {
        case 0:
            sequencia_2 = 0;
            estado = 1;
            break;
         
        case 1:
            sequencia_2 = 1;
            estado = 0;
            break;
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