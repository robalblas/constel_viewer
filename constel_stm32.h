/****************************************************
 *
 * Constellation viewer.
 *   header file 
 *
 ****************************************************/
#ifndef CONSTEL_STM32_HDR

#define CONSTEL_STM32_HDR
#define USE_ARDUINO 1

#define SPI1_NSS_PIN PA4    //SPI_1 Chip Select pin is PA4. You can change it to the STM32 pin you want.
#define SPI1_DC_PIN PB1    
#define RESET_PIN PB0    

#define INCVAL 8                       // increment val.

#define MIN(a,b) ((a) < (b)? (a) : (b))
#define MAX(a,b) ((a) > (b)? (a) : (b))
#define ABS(a) ((a)<0? (-1*(a)) : (a))

#endif
