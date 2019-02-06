/****************************************************
 *
 * Constellation viewer.
 *   Setup dual ADC mode; channels at PA0 and PA1
 *
 ****************************************************/
#include <STM32ADC.h>

// zie ~/proj/avr/usart_spi/const_stm32.c
#define OFFSET_ADC1  0x40012400 
#define OFFSET_ADC2  0x40012800 
#define OFFSET_GPIOC 0x40011000
#define OFFSET_RCC   0x40021000
long *ADC1_SR   = (long *)(OFFSET_ADC1+0x00);
long *ADC1_CR1  = (long *)(OFFSET_ADC1+0x04);
long *ADC1_CR2  = (long *)(OFFSET_ADC1+0x08);
long *ADC1_SQR3 = (long *)(OFFSET_ADC1+0x34);
long *ADC1_DR   = (long *)(OFFSET_ADC1+0x4c);
long *ADC2_SR   = (long *)(OFFSET_ADC2+0x00);
long *ADC2_CR1  = (long *)(OFFSET_ADC2+0x04);
long *ADC2_CR2  = (long *)(OFFSET_ADC2+0x08);
long *ADC2_SQR3 = (long *)(OFFSET_ADC2+0x34);
long *ADC2_DR   = (long *)(OFFSET_ADC2+0x4c);

void setup_iq_adc()
{

  (*ADC1_CR1)  |=0x060000;    // dualmod: regular sim.
  (*ADC1_CR2)  |=0x000001;    // ADON: enable ADC1
  (*ADC2_CR2)  |=0x000001;    // ADON: enable ADC2
  (*ADC1_SQR3) |=0x00;        // channel 0, pin IN0 = PA0
  (*ADC2_SQR3) |=0x01;        // channel 1, pin IN1 = PA1

  (*ADC1_CR2)  |=0x0e0000;    // set trigger to SWSTART
  (*ADC2_CR2)  |=0x0e0000; 

//  pinMode(TEST0, INPUT_PULLUP); // test
//  pinMode(TEST1, INPUT_PULLUP); // test
}
