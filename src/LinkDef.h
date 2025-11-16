// src/LinkDef.h
#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

// CPNRDataModel.h에 정의된 클래스들을 링크합니다.
#pragma link C++ class ArrayS+;
#pragma link C++ class EventInfo+;
#pragma link C++ class AbsChannel+;
#pragma link C++ class AChannel+;
#pragma link C++ class AChannelData+;
#pragma link C++ class FChannel+;
#pragma link C++ class FChannelData+;

#endif