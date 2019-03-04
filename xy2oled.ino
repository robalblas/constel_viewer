/****************************************************
 *
 * Constellation viewer.
 *   Translation of I/Q voltage to X/Y position at OLED
 *
 ****************************************************/
#include <malloc.h>

// Clip to max. OLED resolution
int clip(int n)
{
  if (n > XY_MAX) n=XY_MAX;
  if (n < 0)      n=0;
  return n; 
}

// Auto calibrate X and Y
//   do_cal    : calibrate
//   freeze_cal: freeze offset/slope
//   do_dbg: show cal. numbers
//   sldrmpl: only adapt slope/offset if slope changes more than this value.
// Note: doesn't work properly (yet).
//
#define CNTMAX 10000
void cal_xy(int *x,int *y,bool do_cal,bool freeze_cal,bool do_dbg)
{
  static int xymin,xymax;
  static int cnt;
  static int xyoffset,xyoffset_use=0;
  static float xyslope=0.,xyslope_use=1.;

  xymin=MIN(xymin,(*x));
  xymin=MIN(xymin,(*y));

  xymax=MAX(xymax,(*x));
  xymax=MAX(xymax,(*y));

  if (xymax<=xymin) xymax=xymin+1;
  cnt++;

  if ((cnt>=CNTMAX)  || (xyslope==0.))
  {
    if ((!freeze_cal)  || (xyslope==0.))
    {
      xyoffset=xymin;
      xyslope=(float)XY_MAX/(float)(xymax-xymin);
      xyoffset_use=xyoffset;
      xyslope_use=xyslope;
    }
    cnt=0;
    if (do_dbg)
    {
      str2oled(1,7,"o=%d",xyoffset_use);
      str2oled(1,5,"s=%f",xyslope_use);
      str2oled(1,3,"xymi=%d",xymin);
      str2oled(1,1,"xyma=%d",xymax);
    } 
    xymin=8192; xymax=0;
  }
  if (do_cal)
  {
    *x=((*x)-xyoffset_use)*xyslope_use;
    *y=((*y)-xyoffset_use)*xyslope_use;
  }
}


// Collect/integrate (x,y) pairs
// After integration: copy array content to OLED
// Max. luminance is 32 (# bits for green: 5)
// Resol. 64x64 (128x128 too big for stm32f103)
#define DISPL_WIDTH 128
#define SARR 64                    // size mem: SARR x SARR (max. 128)
#define DDSARR (DISPL_WIDTH/SARR)  // size dot

#define INCVAL 2                   // increment val.
#define LUMMAX 31                  // clip to max. luminance value
// Pixel at (x,y)
// integrlen=# integration steps
// incval = slope integration
// note: LUMMAX reached after LUMMAX/incval steps.
void coll_write_pix2oled(int x,int y,int integrlen,int incval,int rgb)
{
  static unsigned char *xyarr,*p;
  static int integr;
  char www[DDSARR*2];
  int xx,yy,xxx,yyy;
  unsigned char word[2],r,g,b,lum;

  // allocate mem for SARR x SARR pixels
  if (xyarr==NULL) xyarr=(unsigned char *)malloc(SARR*SARR);

  // size x,y to mem-size
  x/=DDSARR;
  y/=DDSARR;

  // pointer to mem for pos. (x,y)
  p=xyarr+y*SARR+x;

  // increment pos. (x,y), clip to XY_MAX
  if (*p >= (LUMMAX-incval))
    *p=LUMMAX;
  else
    (*p)+=incval;

  integr++;

  // write mem to display afer integration time
  if (integr>=integrlen)
  {
    write_cmdbyt2(0x15,0,XY_MAX);                  // set OLED start/end X
    write_cmdbyt2(0x75,0,XY_MAX);                  // set OLED start/end Y
    write_cmd(0x5C);                               //
    digitalWrite(SPI1_NSS_PIN, LOW);               // set CSN active for SPI transm.

    for (yy=0; yy<SARR; yy++)                      // mem Y dir.
    {
      for (yyy=0; yyy<DDSARR; yyy++)               // # y per dot; repeat or DDSARR lines
      {
        for (xx=0; xx<SARR; xx++)                  // mem X dir.; one line
        {  
          p=xyarr+yy*SARR+xx;                      // get value from mem
          lum=*p;

          r=lum>>1;                                // red   value: 0...15 (lum: 0...31)
          g=lum>>0;                                // green value: 0...31
          b=lum>>1;                                // blue  value: 0...15
          if (!(rgb&4)) r=0;
          if (!(rgb&2)) g=0;
          if (!(rgb&1)) b=0;
          word[0]=((b<<3)&0xf8) + ((g>>3)&0x0f);   // set LSByte word
          word[1]=((g<<5)&0xe0) + ((r&0x1f));      // set MSByte word

          for (xxx=0; xxx<DDSARR; xxx++)           // copy lum for one dot-line in array
          {
            www[xxx*2]=word[0];
            www[xxx*2+1]=word[1];
          }
          SPI.write(www,DDSARR*2);                 // transmit 1 line of 1 dot to OLED
        }
      }
    }
    digitalWrite(SPI1_NSS_PIN, HIGH);              // set CSN inactive for SPI transm.

     // reset integration and array
    integr=0;
    memset(xyarr,0,SARR*SARR);                     // reset array for next integration
  }
}
