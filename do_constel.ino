/****************************************************
 *
 * Constellation viewer.
 *   Loop-function 
 *
 ****************************************************/
#define DEBUGn
#define AUTO_SIZING

#define EOC_BIT 0x02
#define SWSTART_BIT 0x00400000

#define TEST0 PB12      // 
#define TEST1 PB13      // 

#define DEBUG 0         // define 1 or 2 for debugging

// To add pins for trying/debugging. May be added to do_constel; will just run one time.
void set_debug()
{
  static int done;
  if (done) return;
  pinMode(TEST0, INPUT_PULLUP); // test
  pinMode(TEST1, INPUT_PULLUP); // test
  done=1;
}

// The loop function
void do_constel()
{
  int x,y;
  int integrlen=8192;
  int incval=INCVAL;
  float drmpl=0.1;

#if DEBUG
  set_debug();
#endif

  (*ADC1_CR2) |= SWSTART_BIT;   // take new samples
  while (!(*ADC1_SR)&EOC_BIT); // wait until EOC is set

  x=*ADC1_DR;                  // read ADC1
  y=*ADC2_DR;                  // read ADC2

#ifdef AUTO_SIZING
#if DEBUG == 1
  // trial hysteresis to prevent wobbling
  if (digitalRead(TEST0))
    drmpl=0.01;
  else
    drmpl=0.03;

  if (digitalRead(TEST1))
    cal_xy(&x,&y,1,drmpl);
  else
    cal_xy(&x,&y,0,drmpl);
#else
    cal_xy(&x,&y,0,drmpl);
#endif

#else
  x>>=4;
  y>>=4;
  x-=25;
  y-=25;
#endif
  x=clip(x);
  y=clip(y);

#if DEBUG == 2
  // trial integration length and increment values
  if (digitalRead(TEST0)) integrlen=8192; else integrlen=4096;
  if (digitalRead(TEST1)) incval=INCVAL; else incval=16;
#endif

  integrlen=8192;
  incval=INCVAL;
  coll_write_pix2oled(x,y,integrlen,incval);
}
