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

uint8_t duty_LCD1_TECLADO = ' ', duty_LCD2_TECLADO = ' ', duty_LCD3_TECLADO = ' ', duty_LCD4_TECLADO  = ' ';
bool duty_AD = 0;
uint16_t duty_teclado[4] = {0};
uint8_t i = 0, dado1 = 0, dado2 = 0, dado3 = 0, dado4 = 0,  memoria_dado = 0;
uint16_t pwm_ventoinha = 0;
uint16_t porcentagem = 0;
uint8_t buffer[10] ={0};
    
void f_pwm(uint16_t duty);
void loop(void);
void read_keyboard (void);
void show_to_LCD(void);
void read_keyb(void);
void f_int0(void);
void f_int1(void);
void f_int2(void);
void Read_AD(void);

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

     
}

void read_keyb(void)
{
    uint8_t dado, dado_RB3, dado_RB4, dado_RB5;
    
    static uint8_t m_dado_RB3 = 1, m_dado_RB4 = 1, m_dado_RB5 = 1;
    
    dado = PORTB;
    
    
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
    
    if (dado_RB3< m_dado_RB3)//li RB3 -> 0 - 300ms, 1-> 600ms
    {
        f_int0();
    }

    if (dado_RB4 < m_dado_RB4) //li RB4 -> 0 - 300ms, 1-> 600ms
    {
        f_int1();
    }
    
    if (dado_RB5 < m_dado_RB5) //li RB4 -> 0 - 300ms, 1-> 600ms
    {
        f_int2();
    }
    
    
    m_dado_RB3 = dado_RB3;
    m_dado_RB4 = dado_RB4;
    m_dado_RB5 = dado_RB5;
}

void show_to_LCD (void) 
{
    /*
    1023 - 100%
    pwm    - v%
     
    v% = 100*pwm/1023
     
    */
    lcd_cmd(L_L1);
    lcd_dat(duty_LCD1_TECLADO);
    lcd_dat(duty_LCD2_TECLADO);
    lcd_dat(duty_LCD3_TECLADO);
    lcd_dat(duty_LCD4_TECLADO);
    /*static unsigned char ind1=0, mind1=9;
    
    lcd_cmd(L_L1);
    linha1[mind1]=linha2[mind1];
    linha1[ind1]=32;
    lcd_str(linha1);
    mind1 = ind1;
    
    
    if (ind1<9) ind1++; else ind1=0;*/
    
}


void read_keyboard (void)
{
    
    static uint8_t dado;
    
    dado = ler_teclado();
    
    if (dado!=16 && dado != memoria_dado)
    {
        if(dado == 11 || dado == 10)
        {
            dado1 = dado2 = dado3 = dado4 = 0;
            memoria_dado = 0;
            i = 0;
            duty_LCD1_TECLADO = ' ';
            duty_LCD2_TECLADO = ' ';
            duty_LCD3_TECLADO = ' ';
            duty_LCD4_TECLADO = ' ';
            for(int i = 0; i < 3; i++)
            {
                duty_teclado[i] = 0;
            }
            return;
        }
        switch (i)
        {
            case 0:
                dado1 = dado;
                duty_teclado[i] = dado1;
                duty_LCD1_TECLADO = '0' + duty_teclado[i];
                display1 = dado1;
                display2 = dado2;
                display3 = dado3;
                display4 = dado4;
                break;
            
            case 1:
                dado2 = dado;
                duty_teclado[i] = dado2;
                duty_LCD2_TECLADO = '0' + duty_teclado[i];
                display1 = dado1;
                display2 = dado2;
                display3 = dado3;
                display4 = dado4;
                break;
                
            case 2:
                dado3 = dado;
                duty_teclado[i] = dado3;
                duty_LCD3_TECLADO = '0' + duty_teclado[i];
                display1 = dado1;
                display2 = dado2;
                display3 = dado3;
                display4 = dado4;
                break;
            
            case 3:
                dado4 = dado;
                duty_teclado[i] = dado4;
                duty_LCD4_TECLADO = '0' + duty_teclado[i];
                display1 = dado1;
                display2 = dado2;
                display3 = dado3;
                display4 = dado4;
                break;
            
        }
        if(i > 3) i = 0;
        else    i++;
        
    }
    
    
    memoria_dado = dado;
}

void f_int0(void)
{
    pwm_ventoinha = 0;
    lcd_cmd(L_CLR);
    f_pwm(pwm_ventoinha);
    duty_AD = 0;
    lcd_cmd(L_L2);
    lcd_str("0%");
}

void f_int1(void)
{
    duty_AD = 1;
}

void f_int2(void)
{
    static uint16_t duty_normalized = 0;
    duty_AD = 0;
    if(duty_LCD1_TECLADO == ' ' && duty_LCD1_TECLADO == ' ' && duty_LCD1_TECLADO == ' ' && duty_LCD1_TECLADO == ' ')
    {
        return;
    }
    duty_normalized = duty_teclado[0]*1000 + duty_teclado[1]*100 + duty_teclado[2]*10 + duty_teclado[3];
    pwm_ventoinha = duty_normalized;
    if(pwm_ventoinha < 1024)
        f_pwm(pwm_ventoinha);
    porcentagem = (pwm_ventoinha * 100.0) / 1023.0;
    lcd_cmd(L_L2);
    sprintf(buffer, "%d%%", porcentagem);
    lcd_str(buffer);
}

void Read_AD(void)
{
    uint16_t data = AD_data[0], pwm_aux = 0;
    
    pwm_aux  = data<<2;
    display1 = (pwm_aux)/1000;
    display2 = ((pwm_aux)%1000)/100;
    display3 = ((pwm_aux)%100)/10;
    display4 = (pwm_aux)%10;
    
    if(duty_AD)
    {
        porcentagem = (pwm_aux * 100.0) / 1023.0;
        f_pwm(pwm_aux);
        lcd_cmd(L_L2);
        sprintf(buffer, "%d%%", porcentagem);
        lcd_str(buffer);
        duty_AD = 0;
    }
}

