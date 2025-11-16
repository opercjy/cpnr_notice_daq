// hardware/ROOT_Wrappers/NoticeM64ADCROOT.h
#ifndef NKM64ADC_ROOT_H
#define NKM64ADC_ROOT_H

#include "TObject.h"

struct libusb_context;

class NKM64ADC : public TObject {
public:
	
  NKM64ADC();
  virtual ~NKM64ADC();
  int M64ADCopen(int sid, libusb_context *ctx);
  void M64ADCclose(int sid);
  void M64ADCresetTIMER(int sid);
  void M64ADCreset(int sid);
  void M64ADCstart(int sid);
  void M64ADCstop(int sid);
  unsigned long M64ADCread_RUN(int sid);
  void M64ADCwrite_CW(int sid, unsigned long data);
  unsigned long M64ADCread_CW(int sid);
  void M64ADCwrite_DRAMON(int sid, unsigned long data);
  unsigned long M64ADCread_DRAMON(int sid);
  unsigned long M64ADCread_PED(int sid, unsigned long ch);
  void M64ADCwrite_DLY(int sid, unsigned long data);
  unsigned long M64ADCread_DLY(int sid);
  void M64ADCwrite_THR(int sid, unsigned long ch, unsigned long data);
  unsigned long M64ADCread_THR(int sid, unsigned long ch);
  void M64ADCwrite_PSW(int sid, unsigned long data);
  unsigned long M64ADCread_PSW(int sid);
  Int_t M64ADCread_BCOUNT(int sid); // (int -> Int_t)
  void M64ADCwrite_PTRIG(int sid, unsigned long data);
  unsigned long M64ADCread_PTRIG(int sid);
  void M64ADCsend_TRIG(int sid);
  void M64ADCwrite_TM(int sid, unsigned long data);
  unsigned long M64ADCread_TM(int sid);
  void M64ADCwrite_MTHR(int sid, unsigned long data);
  unsigned long M64ADCread_MTHR(int sid);
  void M64ADCsend_ADCRST(int sid);
  void M64ADCsend_ADCCAL(int sid);
  void M64ADCwrite_ADCDLY(int sid, unsigned long ch, unsigned long data);
  void M64ADCwrite_ADCALIGN(int sid, unsigned long data);
  unsigned long M64ADCread_ADCSTAT(int sid);
  void M64ADCwrite_BITSLIP(int sid, unsigned long ch, unsigned long data);
  void M64ADCwrite_FMUX(int sid, unsigned long ch);
  unsigned long M64ADCread_FMUX(int sid);
  void M64ADCarm_FADC(int sid);
  unsigned long M64ADCread_FREADY(int sid);
  void M64ADC_ADCALIGN_64(int sid);
  void M64ADCread_FADCBUF(int sid, unsigned long *data);
  Int_t M64ADCread_DATA(int sid, int bcount, unsigned char * data); // (int -> Int_t)

  ClassDef(NKM64ADC, 0) // NKM64ADC wrapper class for root
};

#endif