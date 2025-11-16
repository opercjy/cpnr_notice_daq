// src/CPNRDataModel.h
#ifndef CPNRDATAMODEL_H
#define CPNRDATAMODEL_H

/**
 * @file CPNRDataModel.h
 * @brief CPNR 프로젝트의 ROOT TTree 저장용 데이터 객체(스키마)를 정의합니다.
 * 이 파일의 클래스들은 TTree에 직접 저장됩니다.
 * (CUPDAQ의 RawObjs 모듈을 대체 및 캡슐화)
 */

#include "TObject.h"
#include "TClonesArray.h"
#include "TArray.h"

//
// 1. ArrayS: FADC 파형 저장을 위한 16-bit short 배열
// (CUPDAQ/RawObjs/ArrayS.hh에서 차용)
//
class ArrayS : public TArray {
public:
    unsigned short * fArray; //[fN] Array of fN shorts

    ArrayS() : fArray(0) {}
    ArrayS(Int_t n) : fArray(0) { Set(n); }
    ArrayS(Int_t n, const unsigned short * array) : fArray(0) { Set(n, array); }
    ArrayS(const ArrayS & array) : TArray(array), fArray(0) { Set(array.fN, array.fArray); }
    ArrayS& operator=(const ArrayS& rhs) { if(this!=&rhs) Set(rhs.fN, rhs.fArray); return *this; }
    virtual ~ArrayS() { delete[] fArray; }

    void Adopt(Int_t n, unsigned short * array) {
        if (fArray) delete[] fArray;
        fN = n;
        fArray = array;
    }
    void AddAt(unsigned short c, Int_t i) { if (!BoundsOk("ArrayS::AddAt", i)) return; fArray[i] = c; }
    void Set(Int_t n) {
        if (n < 0) return;
        if (n != fN) {
            unsigned short * temp = fArray;
            if (n != 0) {
                fArray = new unsigned short[n];
                if (n < fN) { memcpy(fArray, temp, n * sizeof(unsigned short)); }
                else {
                    if (fN > 0) memcpy(fArray, temp, fN * sizeof(unsigned short));
                    if (n > fN) memset(&fArray[fN], 0, (n - fN) * sizeof(unsigned short));
                }
            } else { fArray = 0; }
            if (fN) delete[] temp;
            fN = n;
        }
    }
    void Set(Int_t n, const unsigned short * array) {
        if (fArray && fN != n) { delete[] fArray; fArray = 0; }
        fN = n;
        if (fN == 0) return;
        if (array == 0) return;
        if (!fArray) fArray = new unsigned short[fN];
        memmove(fArray, array, n * sizeof(unsigned short));
    }
    unsigned short At(Int_t i) const { if (!BoundsOk("ArrayS::At", i)) return 0; return fArray[i]; }
    unsigned short& operator[](Int_t i) { if (!BoundsOk("ArrayS::operator[]", i)) { i = 0; } return fArray[i]; }
    unsigned short operator[](Int_t i) const { if (!BoundsOk("ArrayS::operator[]", i)) return 0; return fArray[i]; }
    const unsigned short* GetArray() const { return fArray; }
    unsigned short* GetArray() { return fArray; }
    Double_t GetAt(Int_t i) const { return At(i); }
    void Reset(unsigned short val = 0) {
        if (fArray) {
            for (Int_t i = 0; i < fN; i++) fArray[i] = val;
        }
    }

    ClassDef(ArrayS, 1) // Array of shorts
};

//
// 2. EventInfo: 이벤트 공통 헤더 정보
// (CUPDAQ/RawObjs/EventInfo.hh에서 차용)
//
class EventInfo : public TObject {
public:
    EventInfo() : TObject(), fTrgType(0), fNHit(0), fTrgNum(0), fEvtNum(0), fTrgTime(0) {}
    EventInfo(const EventInfo & info) : TObject(info) {
        fTrgType = info.fTrgType;
        fNHit = info.fNHit;
        fTrgNum = info.fTrgNum;
        fEvtNum = info.fEvtNum;
        fTrgTime = info.fTrgTime;
    }
    virtual ~EventInfo() {}

    void Clear(Option_t * = "") {
        fTrgType = 0; fNHit = 0; fTrgNum = 0; fEvtNum = 0; fTrgTime = 0;
    }
    
    void SetTriggerType(unsigned short type) { fTrgType = type; }
    void SetNHit(unsigned short n) { fNHit = n; }
    void SetTriggerNumber(unsigned int n) { fTrgNum = n; }
    void SetEventNumber(unsigned int n) { fEvtNum = n; }
    void SetTriggerTime(unsigned long t) { fTrgTime = t; }

    unsigned short GetTriggerType() const { return fTrgType; }
    unsigned short GetNHit() const { return fNHit; }
    unsigned int GetTriggerNumber() const { return fTrgNum; }
    unsigned int GetEventNumber() const { return fEvtNum; }
    unsigned long GetTriggerTime() const { return fTrgTime; }

private:
    unsigned short fTrgType;   // 트리거 타입 (Pedestal, Self, Global 등)
    unsigned short fNHit;      // 이 이벤트에 포함된 채널(히트) 수
    unsigned int   fTrgNum;    // TCB의 글로벌 트리거 번호
    unsigned int   fEvtNum;    // 파일에 저장되는 이벤트 번호 (Event Counter)
    unsigned long  fTrgTime;   // TCB의 글로벌 트리거 시간 (ns)

    ClassDef(EventInfo, 1)
};

//
// 3. AbsChannel: 모든 채널 데이터의 부모 클래스
// (CUPDAQ/RawObjs/AbsChannel.hh에서 차용)
//
class AbsChannel : public TObject {
public:
    AbsChannel() : TObject(), fID(0), fBit(0) {}
    AbsChannel(unsigned short id) : TObject(), fID(id), fBit(0) {}
    AbsChannel(const AbsChannel & ch) : TObject(ch) {
        fID = ch.fID;
        fBit = ch.fBit;
    }
    virtual ~AbsChannel() {}

    void SetID(unsigned short id) { fID = id; }
    void SetBit(unsigned short bit) { fBit = bit; }
    unsigned short GetID() const { return fID; }   // 채널의 물리적 ID (PID)
    unsigned short GetBit() const { return fBit; } // 트리거 발생 여부

protected:
    unsigned short fID;  // 채널의 물리적 ID (PID)
    unsigned short fBit; // 트리거 비트

    ClassDef(AbsChannel, 1)
};

//
// 4. AChannel: SMODE (SADC) 채널 데이터 (피크 ADC 값, 시간)
// (CUPDAQ/RawObjs/AChannel.hh에서 차용)
//
class AChannel : public AbsChannel {
public:
    AChannel() : AbsChannel(), fADC(0), fTime(0) {}
    AChannel(unsigned short id) : AbsChannel(id), fADC(0), fTime(0) {}
    AChannel(const AChannel & ch) : AbsChannel(ch) {
        fADC = ch.fADC;
        fTime = ch.fTime;
    }
    virtual ~AChannel() {}

    void SetADC(unsigned int count) { fADC = count; }
    void SetTime(unsigned int time) { fTime = time; }
    unsigned int GetADC() const { return fADC; }
    unsigned int GetTime() const { return fTime; }

protected:
    unsigned int fADC;  // 피크 ADC 값 (Charge)
    unsigned int fTime; // 피크 시간 (Time)

    ClassDef(AChannel, 1)
};

//
// 5. FChannel: FMODE (FADC) 채널 데이터 (파형)
// (CUPDAQ/RawObjs/FChannel.hh에서 차용)
//
class FChannel : public AbsChannel, public ArrayS {
public:
    FChannel() : AbsChannel(), ArrayS(), fPedestal(0), fWaveHis(0) {}
    FChannel(unsigned short id) : AbsChannel(id), ArrayS(), fPedestal(0), fWaveHis(0) {}
    FChannel(unsigned short id, int ndp) : AbsChannel(id), ArrayS(ndp), fPedestal(0), fWaveHis(0) {}
    FChannel(const FChannel & ch) : AbsChannel(ch), ArrayS(ch), fWaveHis(0) {
        fPedestal = ch.fPedestal;
    }
    virtual ~FChannel() {} // fWaveHis는 TTree가 관리하므로 여기서 delete하지 않음 (규칙)

    void SetPedestal(unsigned short ped) { fPedestal = ped; }
    unsigned short GetPedestal() const { return fPedestal; }

    void SetWaveform(const unsigned short * wave) { Set(GetSize(), wave); }
    void SetWaveform(int ndp, const unsigned short * wave) { Set(ndp, wave); }
    void SetWaveform(int i, unsigned short adc) { AddAt(adc, i); }
    const unsigned short* GetWaveform() const { return GetArray(); }
    int GetNdp() const { return GetSize(); }

protected:
    unsigned short fPedestal; // FADC가 계산한 페데스탈
    void* fWaveHis;           //! TH1D* (TTree 저장을 위해 void*로 임시 처리)

    ClassDef(FChannel, 1)
};

//
// 6. AChannelData: SMODE (SADC) 이벤트 (AChannel의 TClonesArray)
// (CUPDAQ/RawObjs/AChannelData.hh에서 차용)
//
class AChannelData : public TClonesArray {
public:
    AChannelData() : TClonesArray("AChannel", 1000), fN(0) {} // 기본 크기 할당
    AChannelData(const AChannelData & data) : TClonesArray(data), fN(0) {} // Deep copy는 TClonesArray가 처리
    virtual ~AChannelData() {}

    virtual void Clear(const Option_t * opt = "") { fN = 0; TClonesArray::Clear(opt); }
    AChannel* Add(unsigned short id) {
        // TClonesArray::New() 또는 emplace_back() 사용이 권장됨
        return new ((*this)[fN++]) AChannel(id); 
    }
    int GetN() const { return GetEntriesFast(); }
    AChannel* Get(int n) const { return (AChannel*)At(n); }
    void CopyFrom(const AChannelData * data) {
        Clear();
        int nch = data->GetN();
        for (int i = 0; i < nch; i++) {
            AChannel * ch = data->Get(i);
            new ((*this)[fN++]) AChannel(*ch);
        }
    }

private:
    int fN; //! (임시 카운터)

    ClassDef(AChannelData, 1)
};

//
// 7. FChannelData: FMODE (FADC) 이벤트 (FChannel의 TClonesArray)
// (CUPDAQ/RawObjs/FChannelData.hh에서 차용)
//
class FChannelData : public TClonesArray {
public:
    FChannelData() : TClonesArray("FChannel", 100), fN(0) {} // 기본 크기 할당
    FChannelData(const FChannelData & data) : TClonesArray(data), fN(0) {} // Deep copy는 TClonesArray가 처리
    virtual ~FChannelData() {}

    virtual void Clear(const Option_t * opt = "") { fN = 0; TClonesArray::Clear(opt); }
    FChannel* Add(unsigned short id, int ndp) {
        return new ((*this)[fN++]) FChannel(id, ndp);
    }
    FChannel* Add(FChannel* ch) {
        return new ((*this)[fN++]) FChannel(*ch);
    }
    int GetN() const { return GetEntriesFast(); }
    FChannel* Get(int n) const { return (FChannel*)At(n); }
    void CopyFrom(const FChannelData * data) {
        Clear();
        int nch = data->GetN();
        for (int i = 0; i < nch; i++) {
            FChannel * ch = data->Get(i);
            new ((*this)[fN++]) FChannel(*ch);
        }
    }

private:
    int fN; //! (임시 카운터)

    ClassDef(FChannelData, 1)
};

#endif // CPNRDATAMODEL_H