/*
 * File:   ADC.c
 * Author: johanns123
 *
 * Created on 1 de Novembro de 2022, 23:34
 */


#include <xc.h>
#include "ADC.h"

void AD_init(void)
{
    ADCON0 = 0b00000001;    //AD0 configurado e AD habilitado
    ADCON1 = 0b00001101;    //pino AN0 como AD
    ADCON2 = 0b10010100;    //Justificado a direita
}

void start_AD_conversion(void)
{
    GO = 1;
}

void select_channel(uint8_t channel)
{
    //channel de 0 a 13
    //0b00001101
    //ADCON = 0b00111100
    ADCON0 = 0b00000001 | (channel << 2); 
}

void ADC_maq(void)
{
    static uint8_t estado = 0;
    
    switch (estado)
    {
        case 0:
            AD_data[0] = ADRES;
            select_channel(1);
            start_AD_conversion();
            estado = 1;
            break;
            
        case 1:
            AD_data[1] = ADRES;
            select_channel(0);   //indico canal
            start_AD_conversion();     //inicio a conversao
            estado = 0; //mudo de estado
            break;
    }
}