/****************************************************
 *
 * Constellation viewer.
 *   Setup SPI for driving OLED; 4.5 Mb/s
 *
 ****************************************************/
#include "constel_stm32.h"

#include <SPI.h>                        // in here: extern SPIClass SPI;
void setup_spi()
{
  // Setup SPI 1
  SPI.begin();                          // Initialize the SPI_1 port.
  SPI.setBitOrder(MSBFIRST);            // Set the SPI_1 bit order
  SPI.setDataMode(SPI_MODE0);           // Set the  SPI_1 data mode 0
  SPI.setClockDivider(SPI_CLOCK_DIV16); // Speed 72 / 16 = 4.5 MHz SPI_1
  pinMode(SPI1_NSS_PIN, OUTPUT);        // CS pin
  pinMode(SPI1_DC_PIN, OUTPUT);         // DC pin (data/control)
  pinMode(RESET_PIN, OUTPUT);
}
