/*
 * File:    main.h
 * 
 * Authors:  Jefferson Henrique de Oliveira Fernandes
 *           Raquel Ferreira de Lima
 *           Eduardo do Nascimento Ferreira 
 * 
 * Created on 26 de Outubro de 2022, 19:20
 */

#include <xc.h>
#include <stdbool.h>
#include <stdio.h>

#define OFF             0
#define ON              1

#define LOW             0
#define HIGH            1

#define DISPLAYS        0
#define ADC             1

#define CRESCENTE       0
#define DECRESCENTE     1

#define LENTO           0
#define RAPIDO          1

#define PRESSED         0
#define CLICK           1

#define AIN0            0
#define AIN1            1
#define FULL            2


/*Funcoes usadas globalmente*/
unsigned char desloca_pra_direita(unsigned char dado);
unsigned char desloca_pra_esquerda(unsigned char dado);
bool leitura_botao_pressed(char select);
bool leitura_botao_click(char select);
void show_display_10bits(unsigned int dado_10_bits);


/*Variaveis de configuracao do timer*/
unsigned int timer_0, timer_1, timer_2, timer_3;

/*Dados de comunicação*/
unsigned char dado_disp_7seg;

/*Displays de 7 segmentos*/
unsigned char display4 = 0, display3 = 0, display2 = 0, display1 = 0;

/*Variaveis LCD*/
unsigned  int duty_LCD1 = ' ', duty_LCD2 = ' ', duty_LCD3 = ' ', duty_LCD4  = ' ', porcentagem = 0, vetor[16] = {0};
unsigned char i = 0, dado1 = 0, dado2 = 0, dado3 = 0, dado4 = 0,  memoria_dado = 0;
unsigned  int duty_teclado[4] = {0};

/*Dado valor_2: Teclado matricial*/
unsigned int valor_teclado = 0;
unsigned int    memoria_ad = 0;

/*Flag de chave geral*/
bool flag_b0 = OFF; 
/*Flags para controlar sentido de contagem*/
bool flag_b1 = CRESCENTE;
bool flag_b2 = CRESCENTE;
/*Leitura do botao3*/
bool leitura_b3 = 0;
/*Leitura do botao4*/
bool leitura_b4 = 0;
/*Leitura do botao5*/
bool leitura_b5 = 0;
/*Flag que seleciona o uso do PORTD*/
bool set_portd = DISPLAYS;

