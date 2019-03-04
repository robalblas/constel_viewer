 Constellation viewer.
 Using STM32_F103 Blue Pill board and 128x128 OLED display.

 Clocks:
 By default, clock settings are: (LeafLabs-style board)
   PLLMUL=7 = *9 ==>  9*8 = 72 MHz (cannot be changed after reset!)
   ADCPRE=2 = /6 ==> 72/6 = 12 MHz (cannot be changed?)

 Analog inputs: Depending on inputs PB13/PB12 offset and range are automatically 
 adapted to the input signals, so the constellation fits to the OLED display.
 If this gives too much "wobbling" the offset and range can be fixed (see PB13/PB12 inputs); 
 in that case potmeters are needed at the analog inputs to manually adjust offset and range.
  

 Pinning:
    Analog inputs:
      PA0    ADC I input
      PA1    ADC Q input
 
    OLED connections:
      CPU    OLED
      -----------------------------
      PA5    SCL (SPI clock)
      PA7    SDA (SPI data output)
      PA4    CS  (SPI chip select)
      PB1    DC  (data/control)
      PB0    RES (reset)
  
    Misc. settings: (note: open='1', connected to GND=0)
      PB13,PB12  00: fixed level  ; analog value / 16    .
                 10: adapted level; analog value / 16    
                 01: fixed level
                 11: adapted level   
 
      PB14       0: debug; show adaption parameters
 
      PA8        0: red off
      PA9        0: green off
      PA10       0: blue off
