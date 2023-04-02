/*
 * File:   adc.c
 * Author: Jefferson Henrique
 *
 * Created on 2 de Novembro de 2022, 11:59
 */

#include <xc.h>
#include "main.h"
#include "adc.h"
#include "disp_7seg.h"
#include "pwm.h"

void start_AD_conversion(void)
{
    GO = 1;
}

unsigned int read_AD_10bits(char channel)
{
    static unsigned int leitura_low, leitura_high, leitura = 0;
    ADCON0 = 0b00000001 | (channel << 2); 
    GO   = 1;
    while(GO);
    
    leitura_low  = ADRESL;
    leitura_high = (ADRESH << 8);
    
    leitura = (leitura_high | leitura_low);
    
    return leitura;
}

unsigned char read_AD_8bits(char mode, char channel)
{
    static unsigned char leitura_low, leitura_high, leitura = 0;
    
    switch(mode)
    {
        case LOW:
            CHS0 = channel; // seta o canal desejado para leitura AD
            GO = 1;
            while(GO);
            leitura_low = ADRESH >> 4;
            break;
        case HIGH:
            CHS0 = channel; // seta o canal desejado para leitura AD
            GO = 1;
            while(GO);
            leitura_high = ADRESH & 0b11110000;
            break;
        case FULL:
            CHS0 = channel;
            GO   = 1;
            while(GO);
            leitura = ADRESH;
    } 
    if(mode != FULL)    leitura = leitura_low | leitura_high;
    
    LATD = leitura;
    
    return leitura;
}

void read_AD_linear(void)
{
    unsigned char leitura;
    
    CHS0 = 0;
    GO   = 1;
    while(GO);
    leitura = ADRESH; 
    conv_binary_linear(leitura);
}

void conv_binary_linear(unsigned char binary)
{
    if(binary == 0)
    {
        LATD = 0b00000000;
    }
    else if(binary <= 32)
    {
        LATD = 0b00000001; 
    }
    else if(binary <= 64)
    {
        LATD = 0b00000011; 
    }
    else if(binary <= 96)
    {
        LATD = 0b00000111;
    }
    else if(binary <= 128)
    {
        LATD = 0b00001111;
    }
    else if(binary <= 160)
    {
        LATD = 0b00011111;
    }
    else if(binary <= 192)
    {
        LATD = 0b00111111;
    }
    else if(binary <= 224)
    {
        LATD = 0b01111111;
    }
    else if(binary <= 256)
    {
        LATD = 0b11111111;
    } 
}
