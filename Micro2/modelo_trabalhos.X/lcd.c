#include "main.h"
#include "lcd.h"
#include "botoes.h"

void show_lcd(void)
{
    static unsigned char tecla, tecla_old = 16;
        
    tecla = ler_teclado();
    
    linha1[17] = "               ";
    
    /*16 = teclado não pressionado*/
    if ((tecla!=16) && (tecla_old == 16))
    {
        linha1[contador_lcd ]  = tecla + 48; // 48 = caractere zero
        linha1[contador_lcd+1] = 0;                   // final de linha
    
        if(contador_lcd < 17) contador_lcd++;
        
        dado_lcd = 10*dado_lcd + tecla;
        
        if((tecla == BOTAO_SUSTENIDO)||tecla == BOTAO_ESTRELA)
        {
            dado_lcd   = 0;
            linha1[0]  = 0;
            contador_lcd = 0;
        }
        lcd_cmd(L_L1);
        lcd_str("               ");
        
        lcd_cmd(L_L1);
        lcd_str(linha1);
        
        valor_teclado = dado_lcd;
    }
    tecla_old = tecla;
}

void rotina_1_lcd(float valor_1)
{
    float porcentagem = (valor_1/1023)*100;
      
    sprintf(linha2,"     %3.2f%%", porcentagem);
    
    lcd_cmd(L_CLR);
    
    lcd_cmd(L_L2);
    lcd_str(linha2);
}

void rotina_2_lcd(char valor_2)
{

}

void rotina_3_lcd(char valor_3)
{

}

void lcd_wr(unsigned char val)
{
  LPORT = val;
}

void lcd_cmd(unsigned char val)
{
	LENA = 1;
        lcd_wr(val);
        LDAT = 0;
        __delay_us(40);
        LENA = 0;
        __delay_us(40);
	LENA = 1;
}
 
void lcd_dat(unsigned char val)
{
	LENA = 1;
        lcd_wr(val);
        LDAT = 1;
        __delay_us(40);
        LENA = 0;
        __delay_us(40);
	LENA = 1;
}

void lcd_init(void)
{
	LENA = 0;
	LDAT = 0;
    __delay_ms(20);
	LENA = 1;
	
	lcd_cmd(L_CFG);
	__delay_ms(5);
	lcd_cmd(L_CFG);
    __delay_ms(1);
	lcd_cmd(L_CFG);     //configura
	lcd_cmd(L_OFF);
	lcd_cmd(L_ON);      //liga
	lcd_cmd(L_CLR);     //limpa
	lcd_cmd(L_CFG);     //configura
    lcd_cmd(L_L1);
    lcd_cmd(L_NCR);
}

void lcd_str(const char* str)
{
 unsigned char i = 0;
  
 while(str[i] != 0 )
 {
   lcd_dat(str[i]);
   i++;
 }  
}






 
