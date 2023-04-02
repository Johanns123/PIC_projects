#ifndef ADC
#define	ADC


#include <xc.h> // include processor files - each processor file is guarded.  
void AD_init(void);
void ADC_maq(void);
void start_AD_conversion(void);
void select_channel(uint8_t channel);
uint16_t AD_data[3] = {0};


#endif	/* XC_HEADER_TEMPLATE_H */

