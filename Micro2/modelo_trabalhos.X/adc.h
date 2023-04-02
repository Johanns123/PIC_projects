/*
 * File:    main.h
 * 
 * Author: Jefferson Henrique de Oliveira Fernandes
 * 
 * Created on 02 de November de 2022, 10:46
 */

// This is a guard condition so that contents of this file are not included
// more than once.

unsigned int ad_data[2];

void start_AD_conversion(void);
unsigned char read_AD_8bits(char mode, char channel);
unsigned int read_AD_10bits(char channel);
void read_AD_linear(void);
void conv_binary_linear(unsigned char binary);
