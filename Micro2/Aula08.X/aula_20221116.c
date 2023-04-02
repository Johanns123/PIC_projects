#include <xc.h>
#include "configbits.h"
#include "lcd.h"

#define _XTAL_FREQ 4000000


#define disp1 LATA2
#define disp2 LATA3
#define disp3 LATA4
#define disp4 LATA5

#define display LATD

unsigned char conv7seg[17] = {
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


unsigned char estado, led1, led2, sel;

unsigned char saida;

void atraso_ms(unsigned int tempo);

void f_int0(void);
void f_int1(void);
void f_int2(void);
void f_timer0 (void);

void loop2 (void);
void loop3 (void);

void ContDisplay7seg (void);
unsigned char ler_teclado(void);
void f_pwm(unsigned int duty);

unsigned char num1=1, num2=2, num3=3, num4=4;



void __interrupt (high_priority) f_interrupt_high (void) {
    
    if (TMR0IE && TMR0IF) {
        TMR0L += 6;
        f_timer0();
        TMR0IF = 0;
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


void __interrupt (low_priority) f_interrupt_low (void) {
    
    
}

void f_timer0 (void) {
    
    static unsigned int tempo=0, tempo2=0, tempo3=0;
    
    if (tempo<4) {
        tempo++;
    }
    else {
        tempo = 0;
        ContDisplay7seg();
    }
    
    if (tempo2<499) {
        tempo2++;
    }
    else {
        tempo2 = 0;
        loop2();
    }
    
    if (tempo3<9) {
        tempo3++;
    }
    else {
        tempo3 = 0;
        loop3();
    }
}

void ContDisplay7seg (void) {
    
    extern unsigned char num1, num2, num3, num4;
    
    static unsigned char estado=1;
    
    switch (estado) {
    
        case 1:
            disp4 = 0;
            display = conv7seg[num1];
            disp1 = 1;
            estado = 2;
            break;
    
        case 2:
            disp1 = 0;
            display = conv7seg[num2];
            disp2 = 1;
            estado = 3;
            break;
            
        case 3:
            disp2 = 0;
            display = conv7seg[num3];
            disp3 = 1;
            estado = 4;
            break;
            
        case 4:
            disp3 = 0;
            display = conv7seg[num4];
            disp4 = 1;
            estado = 1;
            break;
            
        default:
            disp1 = 0;
            disp2 = 0;
            disp3 = 0;
            disp4 = 0;
            estado = 1;
            break;
    
    }
    
}


void setup(void) {
    
    IPEN = 0;
    GIE = 0;
    PEIE = 0;
    
    ADCON1 = 0b00001111;
    
    //porta io
    TRISA = 0b11000011;
    TRISB = 0b00111111;
    TRISC = 0b11111011;
    TRISD = 0b00000000;
    TRISE = 0b11111001;
    
    LATA = 0b00000000;
    LATB = 0b00000000;
    LATD = 0b00000000;
    
    //interrup
    INTCON  = 0b00110000;
    INTCON2 = 0b10000101;
    INTCON3 = 0b11011000;
    
    //easter egg 
    //timer0
    T0CON = 0b11010001;
    TMR0L = 6;
    
    T2CON  = 0b00000101;
    PR2 = 255;
    CCP1CON = 0b00001100;
    
    ADCON0 = 0b00000001;
    ADCON1 = 0b00001101;
    ADCON2 = 0b00010100;
    
    lcd_init();
    
    f_pwm(300);
    
    
    GIE = 1;
    
}


#define delay_teclado 10

unsigned char ler_teclado(void) {
    
    unsigned char entrada, saida=16;
    
    TRISB = 0b11111000;
    TRISD = 0b00001111;
    
    LATB |= 0b00000111; 
    
    LATB ^= 0b00000001;
    for (int i=0; i<delay_teclado; i++) Nop();
    entrada = PORTD;
    
    if ((entrada & 1) == 0) saida=10;
    if ((entrada & 2) == 0) saida=7;
    if ((entrada & 4) == 0) saida=4;
    if ((entrada & 8) == 0) saida=1;
    
    LATB ^= 0b00000011;
    for (int i=0; i<delay_teclado; i++) Nop();
    entrada = PORTD;
    
    if ((entrada & 1) == 0) saida=0;
    if ((entrada & 2) == 0) saida=8;
    if ((entrada & 4) == 0) saida=5;
    if ((entrada & 8) == 0) saida=2;
    
    LATB ^= 0b00000110;
    for (int i=0; i<delay_teclado; i++) Nop();
    entrada = PORTD;
    
    if ((entrada & 1) == 0) saida=11;
    if ((entrada & 2) == 0) saida=9;
    if ((entrada & 4) == 0) saida=6;
    if ((entrada & 8) == 0) saida=3;
    
    TRISB = 0b11111111;
    TRISD = 0b00000000;
       
    return saida;
}


char linha1[17]="ABCDEFGHIJ";
char linha2[17]="ABCDEFGHIJ";

void loop(void) { 
    
    
    
     
}


void loop2 (void) {
    
    static unsigned char ind1=0, mind1=9;
    
    lcd_cmd(L_L1);
    linha1[mind1]=linha2[mind1];
    linha1[ind1]=32;
    lcd_str(linha1);
    mind1 = ind1;
    
    
    if (ind1<9) ind1++; else ind1=0;
    
}


void loop3 (void) {
    
    static unsigned char dado;
    
    dado = ler_teclado();
    
    if (dado!=16) num1=num2=num3=num4=dado;
    
}




void main(void) {
    
   setup();
   
   while(1) loop(); 
}






void f_int0(void) {
    

}

void f_int1(void) {
    
    
}


void f_int2(void) {
    
}


void atraso_ms(unsigned int tempo) {
    
    while(tempo){
        tempo--;
    __delay_ms(1);
    }
}


void f_pwm(unsigned int duty)
{
    CCP1CON &= 0b11001111;  //zero DC1B1 me DC1B0
    CCP1CON |= ((duty & 0b11) << 4);  //atualizo os bits menos significativos
    CCPR1L = duty >> 2; //atualizo os bits mais significativos
}

