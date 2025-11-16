// hardware/ROOT_Wrappers/LinkDef.h
#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

// CPNRController가 사용할 고수준 ROOT 래퍼 클래스만 링크합니다.
#pragma link C++ class NKTCB+;
#pragma link C++ class NKNKFADC500+;
#pragma link C++ class NKM64ADC+;

// (저수준 래퍼인 usb3comroot, usb3tcbroot, NKUSBROOT는
//  CPNRController가 직접 사용하지 않으므로 제외합니다.)

#endif