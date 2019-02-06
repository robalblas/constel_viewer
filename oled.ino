/****************************************************
 *
 * Constellation viewer.
 *   OLED drivers
 *
 ****************************************************/
// OLED functions
#define XY_MAX 127
#include <SPI.h>            // in here defined: extern SPIClass SPI;

// Send one byte as control
void write_cmd(unsigned char data)
{
  digitalWrite(SPI1_DC_PIN, LOW);
  digitalWrite(SPI1_NSS_PIN, LOW);
  delayMicroseconds(10);    //Delay 10 micro seconds.
  SPI.transfer(data);
  digitalWrite(SPI1_NSS_PIN, HIGH);
  digitalWrite(SPI1_DC_PIN, HIGH);
  delayMicroseconds(10);    //Delay 10 micro seconds.
}

// Send one byte as data
void write_byt(unsigned char data)
{
  digitalWrite(SPI1_NSS_PIN, LOW);
  SPI.transfer(data);
  digitalWrite(SPI1_NSS_PIN, HIGH);
}

// Send control and two bytes as data
//   (e.g. for setting range of X or Y)
void write_cmdbyt2(int c,int d1,int d2)
{
  write_cmd(c);
  write_byt(d1);
  write_byt(d2);
}

// Setup OLED: SPI/16-bits mode
void setup_oled()
{
  digitalWrite(RESET_PIN, HIGH);
  delayMicroseconds(100000);    //Delay 10 micro seconds.
  digitalWrite(RESET_PIN, LOW);
  delayMicroseconds(100000);    //Delay 10 micro seconds.
  digitalWrite(RESET_PIN, HIGH);

  // zie SSD1351-doc, blz. 36
  write_cmd(0xFD);    // MCU protection status [reset = 12h]
  write_byt(0x12);    // unlock oled driver

  write_cmd(0xFD);    // MCU protection status [reset = 12h]
  write_byt(0xB1);    // Command A2,B1,B3,BB,BE,C1 accessible if in unlock state

  write_cmd(0xAE);    // Sleep mode On (Display OFF)

  write_cmd(0xB6);    // Set Second Pre-charge Period
  write_byt(0x01);    //   period=1 DCLKS

  write_cmd(0xA2);    // Set vertical scroll by Row (def=0x60)
  write_byt(0x00);    //   

//  write_cmd(0xA0);    // colour depth: 65k
//  write_byt(0x20);    //   

  write_cmd(0xAF);    // Sleep mode OFF (Display ON)
  write_cmd(0x5C);    //
}
