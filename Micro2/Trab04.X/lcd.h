#ifndef LCD
#define LCD

#define LENA  PORTEbits.RE1
#define LDAT  PORTEbits.RE2
#define LPORT PORTD


#define L_ON	0x0F
#define L_OFF	0x08
#define L_CLR	0x01
#define L_L1	0x80
#define L_L2	0xC0
#define L_CR	0x0F		
#define L_NCR	0x0C	

#define L_CFG   0x38

//
// AJUSTE _XTAL_FREQ DE ACORDO COM CLOCK USADO NA PLACA
// AJUSTE TAMBÉM NO ARQUIVO lcd.h
//

#define _XTAL_FREQ 10000000

char linha1[17]="ABCDEFGHIJ";
char linha2[17]="ABCDEFGHIJ";

void lcd_init(void);
void lcd_cmd(unsigned char val); 
void lcd_dat(unsigned char val);
void lcd_str(const char* str);

#endif