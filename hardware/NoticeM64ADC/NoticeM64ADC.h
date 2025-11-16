// hardware/NoticeM64ADC/NoticeM64ADC.h
#ifndef M64ADC_H
#define M64ADC_H

#include <libusb.h>
#ifdef __cplusplus
extern "C" {
#endif

#define M64ADC_VENDOR_ID (0x0547)
#define M64ADC_PRODUCT_ID (0x1503)

extern int M64ADCopen(int sid, libusb_context *ctx);
extern void M64ADCclose(int sid);
extern void M64ADCresetTIMER(int sid);
extern void M64ADCreset(int sid);
extern void M64ADCstart(int sid);
extern void M64ADCstop(int sid);
extern unsigned long M64ADCread_RUN(int sid);
extern void M64ADCwrite_CW(int sid, unsigned long data);
extern unsigned long M64ADCread_CW(int sid);
extern void M64ADCwrite_DRAMON(int sid, unsigned long data);
extern unsigned long M64ADCread_DRAMON(int sid);
extern unsigned long M64ADCread_PED(int sid, unsigned long ch);
extern void M64ADCwrite_DLY(int sid, unsigned long data);
extern unsigned long M64ADCread_DLY(int sid);
extern void M64ADCwrite_THR(int sid, unsigned long ch, unsigned long data);
extern unsigned long M64ADCread_THR(int sid, unsigned long ch);
extern void M64ADCwrite_PSW(int sid, unsigned long data);
extern unsigned long M64ADCread_PSW(int sid);
extern int M64ADCread_BCOUNT(int sid);
extern void M64ADCwrite_PTRIG(int sid, unsigned long data);
extern unsigned long M64ADCread_PTRIG(int sid);
extern void M64ADCsend_TRIG(int sid);
extern void M64ADCwrite_TM(int sid, unsigned long data);
extern unsigned long M64ADCread_TM(int sid);
extern void M64ADCwrite_MTHR(int sid, unsigned long data);
extern unsigned long M64ADCread_MTHR(int sid);
extern void M64ADCsend_ADCRST(int sid);
extern void M64ADCsend_ADCCAL(int sid);
extern void M64ADCwrite_ADCDLY(int sid, unsigned long ch, unsigned long data);
extern void M64ADCwrite_ADCALIGN(int sid, unsigned long data);
extern unsigned long M64ADCread_ADCSTAT(int sid);
extern void M64ADCwrite_BITSLIP(int sid, unsigned long ch, unsigned long data);
extern void M64ADCwrite_FMUX(int sid, unsigned long ch);
extern unsigned long M64ADCread_FMUX(int sid);
extern void M64ADCarm_FADC(int sid);
extern unsigned long M64ADCread_FREADY(int sid);
extern void M64ADC_ADCALIGN_64(int sid);
extern void M64ADCread_FADCBUF(int sid, unsigned long *data);
extern int M64ADCread_DATA(int sid, int bcount, unsigned char *data);

#ifdef __cplusplus
}
#endif

#endif // M64ADC_H