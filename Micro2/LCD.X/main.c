#include <xc.h>
#include <stdbool.h>
#include "configbits.h"
#include "lcd.h"
#include "IO.h"
#include "Interrupt.h"
#include "Display.h"
#include "teclado.h"
#include "ADC.h"

#define _XTAL_FREQ 4000000

uint8_t duty_LCD1 = ' ', duty_LCD2 = ' ', duty_LCD3 = ' ', duty_LCD4  = ' ';
bool duty_AD = 0;
uint16_t duty_teclado[4] = {0};
uint8_t i = 0, dado1 = 0, dado2 = 0, dado3 = 0, dado4 = 0,  memoria_dado = 0;

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
    lcd_cmd(L_L2);
    lcd_str("PWM:");
    lcd_dat(duty_LCD1);
    lcd_dat(duty_LCD2);
    lcd_dat(duty_LCD3);
    lcd_dat(duty_LCD4);
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
        switch (i)
        {
            case 0:
                dado1 = dado;
                duty_teclado[i] = dado1;
                display1 = dado1;
                display2 = dado2;
                display3 = dado3;
                display4 = dado4;
                break;
            
            case 1:
                dado2 = dado;
                duty_teclado[i] = dado2;
                display1 = dado1;
                display2 = dado2;
                display3 = dado3;
                display4 = dado4;
                break;
                
            case 2:
                dado3 = dado;
                duty_teclado[i] = dado3;
                display1 = dado1;
                display2 = dado2;
                display3 = dado3;
                display4 = dado4;
                break;
            
            case 3:
                dado4 = dado;
                duty_teclado[i] = dado4;
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
    display1 = display2= display3 = display4 = 0;
    f_pwm(0);
    duty_LCD1 = '0';
    duty_LCD2 = '0';
    duty_LCD3 = '0';
    duty_LCD4 = '0';
    duty_AD = 0;
    i = 0;
    dado1 = 0, dado2 = 0, dado3 = 0, dado4 = 0;
    memoria_dado = 0;
}

void f_int1(void)
{
    duty_AD = 1;
}

void f_int2(void)
{
    static uint16_t duty_normalized = 0;
    duty_AD = 0;
    duty_normalized = duty_teclado[0]*1000 + duty_teclado[1]*100 + duty_teclado[2]*10 + duty_teclado[3];
    duty_LCD1 = '0' + duty_teclado[0];
    duty_LCD2 = '0' + duty_teclado[1];
    duty_LCD3 = '0' + duty_teclado[2];
    duty_LCD4 = '0' + duty_teclado[3];
    f_pwm(duty_normalized);
}

void Read_AD(void)
{
    uint16_t data = AD_data[0];
    
    if(duty_AD)
    {
        f_pwm(data<<2);
        duty_LCD1 = '0' + (data<<2)/1000;
        duty_LCD2 = '0' + ((data<<2)%1000)/100;
        duty_LCD3 = '0' + ((data<<2)%100)/10;
        duty_LCD4 = '0' + (data<<2)%10;
    }
}

