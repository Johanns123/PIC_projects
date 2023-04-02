/*
 * File:    mian.c
 *  
 * Created on 26 de Outubro de 2022, 19:20
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



#define _XTAL_FREQ  4000000UL   //clock de 4MHz

/*----------Prototipo das funcoes----------*/
#include <stdint.h>         // permite padronizar os tipos de memoria
#include "configbits.h"     // cabeçalho com configurações iniciais
#include "main.h"           // contem o protótipo das funcoes do codigo
#include "setup_config.h"
#include "adc.h"
#include "disp_7seg.h"      // ações do display_7_segmentos
#include "lcd.h"
#include "botoes.h"
#include "pwm.h"

void f_int0(void);
void f_int1(void);
void f_int2(void);
void f_get_timers(void);
void f_timer0(void);
void f_timer1(void);
void f_timer2(void);
void f_timer3(void);
void loop(void);

uint16_t temperature = 0;

void __interrupt (high_priority) f_interrupt_high (void) //função chamada ao ocorrer uma interrupção
{
    if(TMR0IF)
    {                            //Testando se houve estouro no Timer0
        TMR0L += 6;              //Reinicia o registrador Timer0
             
        f_get_timers();          //função do timer em que gerencio várias bases de tempo
                                 //Forma de gerar um processamento em "paralelo"
        TMR0IF = 0;              //limpa a flag para a próxima interrupção
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
    if (ADIF && ADIE)
    {
 
        ADIF = 0;
    }
}

void show_display_10bits(unsigned int dado_10_bits)
{   
    static unsigned int memoria;
    
    memoria = dado_10_bits;  
    
    display1 =  memoria/1000;
    display2 = (memoria%1000)/100;
    display3 = (memoria%100)/10;
    display4 =  memoria%10;
}

int main(void)
{
   setup();
   while(1) loop(); 
   return 0;
}

void f_get_timers(void)
{
    static int counter0 = 1, counter1 = 1, counter2 = 1, counter3;
    
    TRISE0 = 0;
    LATE0 = !LATE0;
    
    if(counter0 < timer_0)    
    {  
        counter0++;                  
    }
        
    else 
    {
        f_timer0();
        counter0 = 1; 
    }
    if(counter1 < timer_1)
    {  
        counter1++;                 
    }

    else 
    {
        f_timer1();
        counter1 = 1;
    }
    
    if(counter2 < timer_2) 
    {  
        counter2++;                
    }

    else 
    {
        f_timer2();
        counter2 = 1;
    }
    
    if(counter3 < timer_3) 
    {  
        counter3++;                  
    }

    else 
    {
        f_timer3();
        counter3 = 1;
    }
}

void loop(void)
{
    show_lcd();
    show_display_10bits(read_AD_10bits(AIN1));
} 

void f_int0(void) 
{   
    /*Troca o estado da flag sempre que precionado o botao*/
    flag_b0 = !flag_b0;
}

void f_int1(void)
{
    /*Troca o estado da flag sempre que precionado o botao*/
    flag_b1 = !flag_b1;
}

void f_int2(void)
{
    /*Troca o estado da flag sempre que precionado o botao*/
    flag_b2 = !flag_b2;
}

void f_timer0(void) /* 05 ms */
{   
//    rotina_teclado();
//    show_lcd();
//    show_display_10bits(read_AD_10bits(AIN1));
    mux_display();
}

void f_timer1(void) /* 10 ms */
{   
    setup_bottons(PRESSED, PRESSED, PRESSED);   
    read_AD_10bits(AIN1);
    
}
void f_timer2(void) /* 20 ms */
{       
    
    porcentagem = (memoria_ad/1023)*100;
    
    if(leitura_b3)
    {        
        memoria_ad = 0;
        pwm_dutycycle(memoria_ad);
        rotina_1_lcd(memoria_ad);
    }
    if(leitura_b4)
    {
        memoria_ad = read_AD_10bits(AIN1);
        pwm_dutycycle(memoria_ad);
        rotina_1_lcd(memoria_ad);
    }
    if(leitura_b5 && (valor_teclado <= 1023))
    {
        memoria_ad = valor_teclado;
        pwm_dutycycle(memoria_ad);
        rotina_1_lcd(memoria_ad);
        
        dado_lcd        = 0;
        linha1[0]       = 0;
        contador_lcd    = 0;
    }
}
void f_timer3(void) /* 40 ms */
{   

}

void read_temperature(void)
{   
    static uint16_t LM35 = 0;
    LM35 = read_AD_10bits(2);
    temperature = (500.0*LM35)/1023.0;
} 


