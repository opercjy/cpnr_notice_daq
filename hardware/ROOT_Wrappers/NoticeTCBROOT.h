// hardware/ROOT_Wrappers/NoticeTCBROOT.h
#ifndef TCBROOT_H
#define TCBROOT_H

#include "TObject.h"
// nkusbconst.h는 NoticeTCB.h -> usb3tcb.h -> ... 를 통해 간접 포함될 수 있으나,
// 명시적으로 포함하는 것이 좋습니다.
// #include "nkusbconst.h" // (nkusb.h 헤더가 이미 포함됨)

struct libusb_context;
//struct libusb_device;
//struct libusb_device_handle;

class NKTCB : public TObject {
public:
    
    NKTCB();
    virtual ~NKTCB();

    int TCBopen(int sid, libusb_context *ctx);
    void TCBclose(int sid);
    void TCBwrite_LT(int sid, unsigned long mid, char* data, int len);
    void TCBreset(int sid);
    void TCBresetTIMER(int sid);
    void TCBstart(int sid);
    void TCBstop(int sid);
    unsigned long TCBread_RUN(int sid, unsigned long mid);
    void TCBwrite_CW(int sid, unsigned long mid, unsigned long ch, unsigned long data);
    unsigned long TCBread_CW(int sid, unsigned long mid, unsigned long ch);
    void TCBwrite_RL(int sid, unsigned long mid, unsigned long data);
    unsigned long TCBread_RL(int sid, unsigned long mid);
    void TCBwrite_GW(int sid, unsigned long mid, unsigned long data);
    unsigned long TCBread_GW(int sid, unsigned long mid);
    void TCBwrite_DRAMON(int sid, unsigned long mid, unsigned long data);
    unsigned long TCBread_DRAMON(int sid, unsigned long mid);
    void TCBwrite_DACOFF(int sid, unsigned long mid, unsigned long ch, unsigned long data);
    unsigned long TCBread_DACOFF(int sid, unsigned long mid, unsigned long ch);
    void TCBmeasure_PED(int sid, unsigned long mid, unsigned long ch);
    unsigned long TCBread_PED(int sid, unsigned long mid, unsigned long ch);
    void TCBwrite_DLY(int sid, unsigned long mid, unsigned long ch, unsigned long data);
    unsigned long TCBread_DLY(int sid, unsigned long mid, unsigned long ch);
    void TCBwrite_AMOREDLY(int sid, unsigned long mid, unsigned long ch, unsigned long data);
    unsigned long TCBread_AMOREDLY(int sid, unsigned long mid, unsigned long ch);
    void TCBwrite_THR(int sid, unsigned long mid, unsigned long ch, unsigned long data);
    unsigned long TCBread_THR(int sid, unsigned long mid, unsigned long ch);
    void TCBwrite_POL(int sid, unsigned long mid, unsigned long ch, unsigned long data);
    unsigned long TCBread_POL(int sid, unsigned long mid, unsigned long ch);
    void TCBwrite_PSW(int sid, unsigned long mid, unsigned long ch, unsigned long data);
    unsigned long TCBread_PSW(int sid, unsigned long mid, unsigned long ch);
    void TCBwrite_AMODE(int sid, unsigned long mid, unsigned long ch, unsigned long data);
    unsigned long TCBread_AMODE(int sid, unsigned long mid, unsigned long ch);
    void TCBwrite_PCT(int sid, unsigned long mid, unsigned long ch, unsigned long data);
    unsigned long TCBread_PCT(int sid, unsigned long mid, unsigned long ch);
    void TCBwrite_PCI(int sid, unsigned long mid, unsigned long ch, unsigned long data);
    unsigned long TCBread_PCI(int sid, unsigned long mid, unsigned long ch);
    void TCBwrite_PWT(int sid, unsigned long mid, unsigned long ch, unsigned long data);
    unsigned long TCBread_PWT(int sid, unsigned long mid, unsigned long ch);
    void TCBwrite_DT(int sid, unsigned long mid, unsigned long ch, unsigned long data);
    unsigned long TCBread_DT(int sid, unsigned long mid, unsigned long ch);
    void TCBwrite_PSS(int sid, unsigned long mid, unsigned long ch, unsigned long data);
    unsigned long TCBread_PSS(int sid, unsigned long mid, unsigned long ch);
    void TCBwrite_RT(int sid, unsigned long mid, unsigned long ch, unsigned long data);
    unsigned long TCBread_RT(int sid, unsigned long mid, unsigned long ch);
    void TCBwrite_SR(int sid, unsigned long mid, unsigned long ch, unsigned long data);
    unsigned long TCBread_SR(int sid, unsigned long mid, unsigned long ch);
    void TCBwrite_DACGAIN(int sid, unsigned long mid, unsigned long ch, unsigned long data);
    unsigned long TCBread_DACGAIN(int sid, unsigned long mid, unsigned long ch);
    void TCBwrite_TM(int sid, unsigned long mid, unsigned long ch, unsigned long data); 
    unsigned long TCBread_TM(int sid, unsigned long mid, unsigned long ch);
    void TCBwrite_TLT(int sid, unsigned long mid, unsigned long data); 
    unsigned long TCBread_TLT(int sid, unsigned long mid);
    void TCBwrite_STLT(int sid, unsigned long mid, unsigned long ch, unsigned long data); 
    unsigned long TCBread_STLT(int sid, unsigned long mid, unsigned long ch); 
    void TCBwrite_ZEROSUP(int sid, unsigned long mid, unsigned long ch, unsigned long data); 
    unsigned long TCBread_ZEROSUP(int sid, unsigned long mid, unsigned long ch);
    void TCBsend_ADCRST(int sid, unsigned long mid); 
    void TCBsend_ADCCAL(int sid, unsigned long mid); 
    void TCBwrite_ADCDLY(int sid, unsigned long mid, unsigned long ch, unsigned long data); 
    void TCBwrite_ADCALIGN(int sid, unsigned long mid, unsigned long data); 
    unsigned long TCBread_ADCSTAT(int sid, unsigned long mid); 
    void TCBwrite_BITSLIP(int sid, unsigned long mid, unsigned long ch, unsigned long data);
    void TCBwrite_FMUX(int sid, unsigned long mid, unsigned long ch);
    unsigned long TCBread_FMUX(int sid, unsigned long mid);
    void TCBarm_FADC(int sid, unsigned long mid);
    unsigned long TCBread_FREADY(int sid, unsigned long mid);
    void TCBwrite_ZSFD(int sid, unsigned long mid, unsigned long data);
    unsigned long TCBread_ZSFD(int sid, unsigned long mid);
    void TCBwrite_DSR(int sid, unsigned long mid, unsigned long data);
    unsigned long TCBread_DSR(int sid, unsigned long mid);
    void TCBwrite_ST(int sid, unsigned long mid, unsigned long ch, unsigned long data);
    unsigned long TCBread_ST(int sid, unsigned long mid, unsigned long ch);
    void TCBwrite_PT(int sid, unsigned long mid, unsigned long ch, unsigned long data);
    unsigned long TCBread_PT(int sid, unsigned long mid, unsigned long ch);
    void TCBwrite_DRAMDLY(int sid, unsigned long mid, unsigned long ch, unsigned long data);
    void TCBwrite_DRAMBITSLIP(int sid, unsigned long mid, unsigned long ch);
    void TCBwrite_DRAMTEST(int sid, unsigned long mid, unsigned long data);
    unsigned long TCBread_DRAMTEST(int sid, unsigned long mid, unsigned long ch);
	void TCBwrite_DAQMODE(int sid, unsigned long mid, unsigned long data);
	unsigned long TCBread_DAQMODE(int sid, unsigned long mid);
	void TCBwrite_HV(int sid, unsigned long mid, unsigned long ch, float data);
	float TCBread_HV(int sid, unsigned long mid, unsigned long ch);
	float TCBread_TEMP(int sid, unsigned long mid, unsigned long ch);
	void TCBwrite_ADCMUX(int sid, unsigned long mid, unsigned long ch, unsigned long data);
    void TCBread_FADCBUF(int sid, unsigned long mid, unsigned long *data);
    void TCB_ADCALIGN_500(int sid, unsigned long mid);
    void TCB_ADCALIGN_64(int sid, unsigned long mid);
    void TCB_ADCALIGN_DRAM(int sid, unsigned long mid);
    void TCBwrite_RUNNO(int sid, unsigned long data);
    unsigned long TCBread_RUNNO(int sid);
    void TCBwrite_GATEDLY(int sid, unsigned long data);
    unsigned long TCBread_GATEDLY(int sid);
    void TCBsend_TRIG(int sid);
    void TCBread_LNSTAT(int sid, unsigned long *data);
    void TCBread_MIDS(int sid, unsigned long *data);
    void TCBwrite_PTRIG(int sid, unsigned long data);
    unsigned long TCBread_PTRIG(int sid);
    void TCBwrite_TRIGENABLE(int sid, unsigned long mid, unsigned long data);
    unsigned long TCBread_TRIGENABLE(int sid, unsigned long mid);
    void TCBwrite_MTHR_NKFADC500(int sid, unsigned long data);
    unsigned long TCBread_MTHR_NKFADC500(int sid);
    void TCBwrite_PSCALE_NKFADC500(int sid, unsigned long data);
    unsigned long TCBread_PSCALE_NKFADC500(int sid);
    void TCBwrite_MTHR_M64ADC_MUON(int sid, unsigned long data);
    unsigned long TCBread_MTHR_M64ADC_MUON(int sid);
    void TCBwrite_PSCALE_M64ADC_MUON(int sid, unsigned long data);
    unsigned long TCBread_PSCALE_M64ADC_MUON(int sid);
    void TCBwrite_MTHR_M64ADC_LS(int sid, unsigned long data);
    unsigned long TCBread_MTHR_M64ADC_LS(int sid);
    void TCBwrite_PSCALE_M64ADC_LS(int sid, unsigned long data);
    unsigned long TCBread_PSCALE_M64ADC_LS(int sid);
    void TCBwrite_MTHR_MUONDAQ(int sid, unsigned long data);
    unsigned long TCBread_MTHR_MUONDAQ(int sid);
    void TCBwrite_PSCALE_MUONDAQ(int sid, unsigned long data);
    unsigned long TCBread_PSCALE_MUONDAQ(int sid);
    void TCBwrite_EXTOUT(int sid, unsigned long data);
    unsigned long TCBread_EXTOUT(int sid);
    void TCBwrite_GATEWIDTH(int sid, unsigned long data);
    unsigned long TCBread_GATEWIDTH(int sid);
    void TCBwrite_EXTOUTWIDTH(int sid, unsigned long data);
    unsigned long TCBread_EXTOUTWIDTH(int sid);
    unsigned long TCBread_BCOUNT(int sid, unsigned long mid);
    Int_t TCBread_DATA(int sid, unsigned long mid, unsigned long bcount, unsigned char *data); // (int -> Int_t)
    void TCB_ADCALIGN_125(int sid, unsigned long mid);
	void TCB_ADCALIGN_MUONDAQ(int sid, unsigned long mid);
	void TCBwrite_GAIN(int sid, unsigned long mid, unsigned long ch, unsigned long data);
	unsigned long TCBread_GAIN(int sid, unsigned long mid, unsigned long ch);
	void TCBwrite_MTHR(int sid, unsigned long mid, unsigned long data);
	unsigned long TCBread_MTHR(int sid, unsigned long mid);
	void TCBwrite_PSD_DLY(int sid, unsigned long mid, unsigned long data);
	unsigned long TCBread_PSD_DLY(int sid, unsigned long mid);
	void TCBwrite_PSD_THR(int sid, unsigned long mid, float data);
	float TCBread_PSD_THR(int sid, unsigned long mid);
    void TCBwrite_TRIG_SWITCH_NKFADC500(int sid, unsigned long fadc, unsigned long sadc_muon, unsigned long sadc_ls, unsigned long muon_daq);
    unsigned long TCBread_SWITCH_NKFADC500(int sid);
    void TCBwrite_TRIG_SWITCH_M64ADC_MUON(int sid, unsigned long fadc, unsigned long sadc_muon, unsigned long sadc_ls,  unsigned long muon_daq);
    unsigned long TCBread_SWITCH_M64ADC_MUON(int sid);
    void TCBwrite_TRIG_SWITCH_M64ADC_LS(int sid, unsigned long fadc, unsigned long sadc_muon, unsigned long sadc_ls, unsigned long muon_daq);
    unsigned long TCBread_SWITCH_M64ADC_LS(int sid);
    void TCBwrite_TRIG_SWITCH_MUONDAQ(int sid, unsigned long fadc, unsigned long sadc_muon, unsigned long sadc_ls, unsigned long muon_daq);
    unsigned long TCBread_SWITCH_MUONDAQ(int sid);
    unsigned long TCBread_DBG(int sid);

    ClassDef(NKTCB, 0) // NKTCB wrapper class for root
};

#endif // TCBROOT_H