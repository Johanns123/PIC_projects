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

#define _XTAL_FREQ 10000000

uint8_t buffer[10] ={0};
uint16_t temperature = 0, temp = 0;;
uint16_t T1 = 0, T2 = 0, TM = 0;
uint16_t giro = 0, HZ = 0, pwm = 0;

void f_pwm(uint16_t duty);
void loop(void);
void show_to_LCD(void);
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

uint8_t digito1_temperatura = 0, digito2_temperatura = 0, digito3_temperatura = 0, digito4_temperatura = 0;
void loop(void) 
{ 
    display2 = (HZ)/100;
    display3 = ((HZ)%100)/10;
    display4 = (HZ)%10;
    
    read_temperature();
    digito1_temperatura = (temperature)/1000;
    digito2_temperatura = ((temperature)%1000)/100;
    digito3_temperatura = ((temperature)%100)/10;
    digito4_temperatura = (temperature)%10; 
}

void Read_AD(void)
{    
//    pwm_aux  = read_AD_10bits(0);
    T1 = AD_data[0]*100.0/1023.0;
    if(T1<20)
        T1 = 20;
    T2 = AD_data[1]*100.0/1023.0;
    if(T2<20)
        T2 = 20;
    TM = (T1+T2)/2;
}

void read_temperature(void)
{
    //TRISA |= (1<<2);
    temperature = (50000.0*AD_data[2])/1023.0;
    temp = temperature/100;
    //temperature = (500.0*AD_data[2])/1023.0;
//    TRISA &= ~(1<<2);
} 


void imprime_temperatura(void)
{
    static uint8_t old_latd = 0;
    old_latd = LATD;
    lcd_cmd(L_CLR);
    lcd_cmd(L_L1);
    lcd_dat((T1/10) + '0');
    lcd_dat((T1%10) + '0');
    lcd_dat(223);
    lcd_dat('C');
    lcd_str("   ");
    lcd_dat((T2/10) + '0');
    lcd_dat((T2%10) + '0');
    lcd_dat(223);
    lcd_dat('C');
    lcd_cmd(L_L2);
    lcd_dat((TM/10) + '0');
    lcd_dat((TM%10) + '0');
    lcd_dat(223);
    lcd_dat('C');
    lcd_str("   ");
    lcd_dat(digito1_temperatura + '0');
    lcd_dat(digito2_temperatura + '0');
    lcd_dat('.');
    lcd_dat(digito3_temperatura + '0');
    lcd_dat(digito4_temperatura + '0');
    lcd_dat(223); //° -> 1101 1111
    lcd_dat('C');
    LATD = old_latd;
    //LATA2=LATA3=LATA4=LATA5=1;
}

void read_RPM(void)
{
    static uint8_t contagem_de_pulsos = 0;
    
    if(PORTC & (1<<0))
        contagem_de_pulsos++;
    
    if(contagem_de_pulsos > 6)
    {
        giro++;
        contagem_de_pulsos = 0;
    }
}

void calculate_RPM(void)
{
    HZ = giro * 100;  //10ms * 1000 = 1s
    giro = 0;
}

void controlador_ventoinha(void)
{    
    static uint8_t heater = 0;
    
    if(temp < T1)
    {
        RC5 = 1;    //ligo o heater
        f_pwm(0);   //desligo a ventoinha
        heater = 1;
    }
    
    else if(temp > T2)
    {
        RC5 = 0; //desliga o heater
        heater = 0;
    }
    
    if(!heater)
    {
        if(temp > TM)
        {
            RC5 = 0; //desliga o heater
            pwm = 624;
            f_pwm(pwm);    //liga em 100%
        }

        else if(temp < TM)   //se for menor que a media
        {
            RC5 = 0; //desliga o heater
            pwm = 436;
            f_pwm(pwm); //liga em 70%
        }
    }
       
}

/*
 20 - 0
 T - 512
 100 - 1023
 
 * T = 512*100/1023
 * T - AD
 * T = AD*100/1023;
 
 */
/*
 250mV - 25°C

10mV/°C 
 
 1023 - 5V -   500°C
 1 - V
 AD - V - °C
 
 V = 5*AD/1023
 °C = 500*V/5 = 100*V
 
 °C = 100*(5*AD/1023) = 500*AD/1023 
 
 V = 5/1023 = 4,8mV 
*/