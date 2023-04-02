#define LENA  PORTEbits.RE1
#define LDAT  PORTEbits.RE2
#define LPORT PORTD


#define L_ON	0x0F // Liga LCD
#define L_OFF	0x08 // Deslga LCD
#define L_CLR	0x01 // paga a linha
#define L_L1	0x80 // Seta linha 1
#define L_L2	0xC0 // Seta linha 2
#define L_CR	0x0F 
#define L_NCR	0x0C	

#define L_CFG   0x38

//
// AJUSTE _XTAL_FREQ DE ACORDO COM CLOCK USADO NA PLACA
// AJUSTE TAMBÉM NO ARQUIVO lcd.h
//

#define _XTAL_FREQ 4000000

void lcd_init(void);
void lcd_cmd(unsigned char val); 
void lcd_dat(unsigned char val);
void lcd_str(const char* str);

void rotina_1_lcd(float valor_1);
void rotina_2_lcd(char  valor_2);
void rotina_3_lcd(char  valor_3);

void show_lcd(void);

char linha1[17] = "ABCDEFGHIJ";
char linha2[17] = "ABCDEFGHIJ";

unsigned char contador_lcd = 0;
int           dado_lcd = 0;