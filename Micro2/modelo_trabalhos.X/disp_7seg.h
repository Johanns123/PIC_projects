/*
 * File:   disp_7seg.h
 * 
 * Author: Jefferson Henrique de Oliveira Fernandes
 *
 * Created on 30 de Outubro de 2022, 09:07
 */

#define DISP1 LATA4
#define DISP2 LATA5
#define DISP3 LATA2
#define DISP4 LATA3

#define DISPLAY LATD

void mux_display(void);
void show_displays_12(void);
void show_displays_34(void);
unsigned char contagem_dec(void);
unsigned char contagem_hex(void);

