/*
 * File:    main.h
 * 
 * Author: Jefferson Henrique de Oliveira Fernandes
 * 
 * Created on 02 de November de 2022, 10:46
 */

void disable_interrupts(void);
void setup(void);
void setup_hardware(void);
void setup_software(void);
void setup_io(void);
void setup_ad(void);
void setup_interrupt(void);
void setup_timer0(void);
void setup_pwm(void);
void enable_interrupts(void);
void setup_bottons(bool botao3, bool botao4, bool botao5);
