#include "main.h"
#include "pwm.h"

void pwm_dutycycle ( unsigned int dutycycle) 
{ 
    /*Tratando 2/10 bits altos*/
    CCP1CON &= 0b11001111;
    CCP1CON |= ((dutycycle & 0b11) << 4);
    /*Tratando 8/10 bits baixos*/
    CCPR1L = (dutycycle >> 2);       
}