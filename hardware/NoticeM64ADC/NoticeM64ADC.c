// CPNR/hardware/NoticeM64ADC/NoticeM64ADC.c (1/1)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "usb3com.h"
#include "NoticeM64ADC.h"

// open M64ADC
int M64ADCopen(int sid, libusb_context *ctx)
{
  int status;

  status = USB3Open(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, ctx);
  USB3ClaimInterface(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0);

//  M64ADCread_BCOUNT(sid);
//  M64ADCread_BCOUNT(sid);
//  M64ADCread_BCOUNT(sid);
//  M64ADCread_BCOUNT(sid);

  printf("Now M64ADC is ready.\n");

  return status;
}

// close M64ADC
void M64ADCclose(int sid)
{
  USB3ReleaseInterface(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0);
  USB3Close(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid);
}

// reset timer
void M64ADCresetTIMER(int sid)
{
  USB3Write(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0x20000000, 1);
}

// reset data acquisition
void M64ADCreset(int sid)
{
  USB3Write(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0x20000000, 1 << 2);
}

// start data acquisition
void M64ADCstart(int sid)
{
  USB3Write(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0x20000000, 1 << 3);
}

// stop data acquisition
void M64ADCstop(int sid)
{
  USB3Write(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0x20000000, 0 << 3);
}

// read RUN status
unsigned long M64ADCread_RUN(int sid)
{
  return USB3ReadReg(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0x20000000);
}

// write coincidence window
void M64ADCwrite_CW(int sid, unsigned long data)
{
  USB3Write(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0x20000001, data);
}

// read coincidence windows
unsigned long M64ADCread_CW(int sid)
{
  return USB3ReadReg(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0x20000001);
}

// turn on/off DRAM
// 0 = off, 1 = on
void M64ADCwrite_DRAMON(int sid, unsigned long data)
{
  USB3Write(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0x20000003, data);
}

// read DRAM status
unsigned long M64ADCread_DRAMON(int sid)
{
  return USB3ReadReg(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0x20000003);
}

// read pedestal
unsigned long M64ADCread_PED(int sid, unsigned long ch)
{
  unsigned long addr;

  addr = 0x20000006 + (((ch - 1) & 0xFF) << 16);
  
  return USB3ReadReg(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, addr);
}

// write input delay
void M64ADCwrite_DLY(int sid, unsigned long data)
{
  unsigned long value;

  value = ((data / 1000) << 10) | (data % 1000);
  USB3Write(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0x20000007, value);
}

// read input delay
unsigned long M64ADCread_DLY(int sid)
{
  unsigned long value;
  unsigned long data;

  value = USB3ReadReg(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0x20000007);
  data = (value >> 10) * 1000 + (value & 0x3FF);
  
  return data;
}

// write discriminator threshold
void M64ADCwrite_THR(int sid, unsigned long ch, unsigned long data)
{
  unsigned long addr;

  addr = 0x20000008 + (((ch - 1) & 0xFF) << 16);

  USB3Write(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, addr, data);
}

// read discriminator threshold
unsigned long M64ADCread_THR(int sid, unsigned long ch)
{
  unsigned long addr;

  addr = 0x20000008 + (((ch - 1) & 0xFF) << 16);
  
  return USB3ReadReg(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, addr);
}

// write pulse sum width
void M64ADCwrite_PSW(int sid, unsigned long data)
{
  USB3Write(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0x2000000A, data);
}

// read pulse sum width
unsigned long M64ADCread_PSW(int sid)
{
  return USB3ReadReg(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0x2000000A);
}

// Read data buffer count, 1 buffer count = 1 kbyte data
int M64ADCread_BCOUNT(int sid)
{
  return (int)USB3ReadReg(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0x20000010);
}

// write pedestal trigger interval in ms;
void M64ADCwrite_PTRIG(int sid, unsigned long data)
{
  USB3Write(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0x20000011, data);
}

// read pedestal trigger interval in ms;
unsigned long M64ADCread_PTRIG(int sid)
{
  return USB3ReadReg(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0x20000011);
}

// send trigger
void M64ADCsend_TRIG(int sid)
{
  USB3Write(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0x20000012, 0);
}

// write trigger mode
void M64ADCwrite_TM(int sid, unsigned long data)
{
  USB3Write(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0x20000014, data);
}

// read trigger mode
unsigned long M64ADCread_TM(int sid)
{
  return USB3ReadReg(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0x20000014);
}

// set multiplicity threshold
void M64ADCwrite_MTHR(int sid, unsigned long data)
{
  USB3Write(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0x20000015, data);
}

// read multiplicity threshold
unsigned long M64ADCread_MTHR(int sid)
{
  return USB3ReadReg(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0x20000015);
}

// send ADC reset signal
void M64ADCsend_ADCRST(int sid)
{
  USB3Write(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0x20000017, 0);
}

// send ADC calibration signal
void M64ADCsend_ADCCAL(int sid)
{
  USB3Write(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0x20000018, 0);
}

// write ADC calibration delay
void M64ADCwrite_ADCDLY(int sid, unsigned long ch, unsigned long data)
{
  unsigned long addr;

  addr = 0x20000019 + (((ch - 1) & 0xFF) << 16);

  USB3Write(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, addr, data);
}

// write ADC align delay
void M64ADCwrite_ADCALIGN(int sid, unsigned long data)
{
  USB3Write(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0x2000001A, data);
}

// read ADC status
unsigned long M64ADCread_ADCSTAT(int sid)
{
  return USB3ReadReg(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0x2000001A);
}

// write ADC bitslip
void M64ADCwrite_BITSLIP(int sid, unsigned long ch, unsigned long data)
{
  unsigned long addr;

  addr = 0x2000001B + (((ch - 1) & 0xFF) << 16);

  USB3Write(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, addr, data);
}

// write FADC buffer mux
void M64ADCwrite_FMUX(int sid, unsigned long ch)
{
  USB3Write(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0x2000001C, ch);
}

// read FADC buffer mux
unsigned long M64ADCread_FMUX(int sid)
{
  return USB3ReadReg(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0x2000001C);
}

// arm FADC buffer
void M64ADCarm_FADC(int sid)
{
  USB3Write(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0x2000001D, 0);
}

// read FADC buffer status
unsigned long M64ADCread_FREADY(int sid)
{
  return USB3ReadReg(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 0x2000001D);
}

// align M64ADC
void M64ADC_ADCALIGN_64(int sid)
{
  unsigned long ch, dly, value;
  int count, sum, center;
  unsigned long bitslip;
  unsigned long gdly;
  unsigned long gbitslip;
  int flag;

  M64ADCsend_ADCRST(sid); 
  usleep(500000);
  M64ADCsend_ADCCAL(sid); 

  for (ch = 1; ch <= 4; ch++) {
    count = 0;
    sum = 0;
    flag = 0;
    gbitslip = 0;

    // ADC initialization codes
    M64ADCwrite_ADCALIGN(sid, 0x030002); 
    usleep(100);
    M64ADCwrite_ADCALIGN(sid, 0x010010); 
    usleep(100);
    M64ADCwrite_ADCALIGN(sid, 0xC78001); 
    usleep(100);
    M64ADCwrite_ADCALIGN(sid, 0xDE01C0); 
    usleep(100);

    // set deskew pattern
    M64ADCwrite_ADCALIGN(sid, 0x450001); 

    // set bitslip = 0
    M64ADCwrite_BITSLIP(sid, ch, 0); 

    for (dly = 0; dly < 32; dly++) {
      M64ADCwrite_ADCDLY(sid, ch, dly); 
      value = (M64ADCread_ADCSTAT(sid) >> (ch -1)) & 0x1; 
      
      // count bad delay
      if (!value) {
        flag = 1;
        count = count + 1;
        sum = sum + dly;
      }
      else {
        if (flag)
          dly = 32;
      }
    }

    // get bad delay center
    center = sum / count;

    // set good delay
    if (center < 9)
      gdly = center + 9;
    else
      gdly = center - 9;

    // sets delay
    M64ADCwrite_ADCDLY(sid, ch, gdly); 

    // set sync pattern
    M64ADCwrite_ADCALIGN(sid, 0x450002); 
    usleep(100);

    for (bitslip = 0; bitslip < 12; bitslip++) {
      M64ADCwrite_BITSLIP(sid, ch, bitslip); 

      value = (M64ADCread_ADCSTAT(sid) >> ((ch -1) + 4)) & 0x1; 
      if (value) {
        gbitslip = bitslip;
        bitslip = 12;
      }
    }

    // set good bitslip
    M64ADCwrite_BITSLIP(sid, ch, gbitslip); 

    printf("ch%ld calibration delay = %ld, bitslip = %ld\n", ch, gdly, gbitslip);
  }

  // set normal ADC mode
  M64ADCwrite_ADCALIGN(sid, 0x450000); 
  usleep(100);
  M64ADCsend_ADCCAL(sid); 
}

// read FADC buffer
void M64ADCread_FADCBUF(int sid, unsigned long *data)
{
  unsigned char rdat[8196]; // (char -> unsigned char)
  unsigned long i;
  unsigned long tmp;

  USB3Read(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, 2049, 0x20008000, rdat);  

  for (i = 0; i < 2048; i++) {
    data[i] = rdat[4 * i] & 0xFF;
    tmp = rdat[4 * i + 1] & 0xFF;
    data[i] = data[i] + (unsigned long)(tmp << 8);
    tmp = rdat[4 * i + 2] & 0xFF;
    data[i] = data[i] + (unsigned long)(tmp << 16);
    tmp = rdat[4 * i + 3] & 0xFF;
    data[i] = data[i] + (unsigned long)(tmp << 24);
  }
}

// read data, reads bcount * 1 kbytes data from M64ADC DRAM
// returns character raw data, needs sorting after data acquisition
int M64ADCread_DATA(int sid, int bcount, unsigned char * data) // (char * -> unsigned char *)
{
  int count;

  // maximum data size is 64 Mbyte
  count = bcount * 256; // 1 bcount = 1kbyte = 256 * 4 bytes

  return USB3Read(M64ADC_VENDOR_ID, M64ADC_PRODUCT_ID, sid, count, 0x40000000, data);  
}