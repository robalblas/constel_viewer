/****************************************************
 *
 * Constellation viewer.
 *   Main part. 
 *
 ****************************************************/
#include "constel_stm32.h"

void setup() 
{
  setup_spi();
  setup_oled();
  setup_iq_adc();
}

void loop()
{
  do_constel();
}

#if USE_ARDUINO == 0
int main(void)
{
  setup();
  while (1)
  {
    loop();
  }
}
#endif
