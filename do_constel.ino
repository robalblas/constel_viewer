/****************************************************
 *
 * Constellation viewer.
 *   Loop-function 
 *
 ****************************************************/

#define EOC_BIT 0x02
#define SWSTART_BIT 0x00400000

#define TEST0 PB12      // 
#define TEST1 PB13      // 
#define TEST2 PB14      // 
#define RGB_R PA8
#define RGB_G PA9
#define RGB_B PA10

#define DEBUG 1         // define 1 or 2 for debugging

// To add pins for trying/debugging. May be added to do_constel; will just run one time.
void set_debug()
{
  static int done;
  if (done) return;
  pinMode(TEST0, INPUT_PULLUP); // test
  pinMode(TEST1, INPUT_PULLUP); // test
  pinMode(TEST2, INPUT_PULLUP); // test
  pinMode(RGB_R, INPUT_PULLUP); // test
  pinMode(RGB_G, INPUT_PULLUP); // test
  pinMode(RGB_B, INPUT_PULLUP); // test
  done=1;
}

// The loop function
// test(1-0)	do_cal	freeze_cal
// 3		true	false		cont. calibrate
// 1		true	true		fixed cal. params
// 2		false	false		no calibration, predefined offset/slope
// 0		false	true		calibration, predefined offset/slope

void do_constel()
{
  int x,y;
  int integrlen=8192;
  int incval=INCVAL;
  bool do_cal=true;
  bool do_dbg=false;
  bool freeze_cal=false;
  int rgb=0x0;

  int test[3];
  test[0]=digitalRead(TEST0);
  test[1]=digitalRead(TEST1);
  test[2]=digitalRead(TEST2);
  if (digitalRead(RGB_R)) rgb|=0x4;
  if (digitalRead(RGB_G)) rgb|=0x2;
  if (digitalRead(RGB_B)) rgb|=0x1;
#if DEBUG
  set_debug();
#endif

  (*ADC1_CR2) |= SWSTART_BIT;   // take new samples
  while (!(*ADC1_SR)&EOC_BIT); // wait until EOC is set

  x=*ADC1_DR;                  // read ADC1
  y=*ADC2_DR;                  // read ADC2

  if (!test[2]) do_dbg=true;
  switch((test[0]+(test[1]<<1))&0x3)
  {
    default:
    case 3: do_cal=true;  freeze_cal=false; break; // cont. calibrate
    case 1: do_cal=true;  freeze_cal=true;  break; // freeze calibration
    case 2: do_cal=false; freeze_cal=false; break; // val. /16, cont. calibrate
    case 0: do_cal=false; freeze_cal=true;  break; // val. /16, no calibration
  }
  if (!do_cal)
  {
    x>>=4;
    y>>=4;
    x-=25;
    y-=25;
  }
  cal_xy(&x,&y,do_cal||(!freeze_cal),freeze_cal,do_dbg);

  x=clip(x);
  y=clip(y);

  integrlen=8192;
  incval=INCVAL;
  coll_write_pix2oled(x,y,integrlen,incval,rgb);
}
