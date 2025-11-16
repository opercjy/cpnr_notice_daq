// src/DataParser.h
#ifndef DATAPARSER_H
#define DATAPARSER_H

#include <vector>
#include <map>
#include <memory>
#include "CPNRDefs.h"       // kBYTESPEREVENTSADC 등 상수
#include "CPNRDataModel.h"  // EventInfo, FChannelData, AChannelData

/**
 * @brief 파싱된 이벤트 데이터 (TTree에 저장하기 전 임시 구조체)
 * 파서가 데이터 버퍼로부터 이벤트들을 추출하여 이 구조체 벡터로 반환합니다.
 * std::unique_ptr를 사용하여 이벤트 객체의 소유권을 CPNRController로 안전하게 이전합니다.
 */
struct ParsedEventData {
    ADC::MODE mode; // 이 이벤트가 FMODE인지 SMODE인지 식별
    
    // TTree에 저장될 객체들
    std::unique_ptr<EventInfo> eventInfo;
    std::unique_ptr<FChannelData> fadcData; // FMODE일 때만 유효
    std::unique_ptr<AChannelData> sadcData; // SMODE일 때만 유효

    ParsedEventData(ADC::MODE m) : mode(m) {
        eventInfo = std::make_unique<EventInfo>();
        if (mode == ADC::FMODE) {
            fadcData = std::make_unique<FChannelData>();
        } else {
            sadcData = std::make_unique<AChannelData>();
        }
    }
};

/**
 * @class DataParser
 * @brief 원시 데이터 버퍼(unsigned char*)를 CPNRDataModel 객체로 변환하는 클래스.
 * (CUPDAQ의 OnlObjs/FADCRawEvent.cc, SADCRawEvent.cc의 Unpack 로직을 캡슐화)
 */
class DataParser {
public:
    DataParser();
    ~DataParser();

    /**
     * @brief 파서가 PID 맵핑, RL 등을 참조할 수 있도록 ADC 설정을 등록합니다.
     * @param mid 모듈 ID
     * @param mode 해당 모듈의 작동 모드 (FMODE/SMODE)
     * @param pids 채널 ID (0-3 또는 0-31) -> 물리적 ID (PID) 맵핑
     * @param nChannels 모듈의 채널 수 (예: 4 또는 32)
     * @param rl Record Length (FMODE인 경우)
     */
    void registerModuleConfig(int mid, ADC::MODE mode, std::map<int, int> pids, int nChannels, int rl = 0);

    /**
     * @brief 원시 데이터 버퍼를 파싱하여 이벤트 목록으로 반환합니다.
     * @param mid 데이터를 생성한 모듈 ID
     * @param buffer TCBread_DATA로 읽어온 버퍼
     * @param bcount 버퍼 크기 (kbytes)
     * @return 파싱된 이벤트 객체(ParsedEventData)의 벡터
     */
    std::vector<ParsedEventData> parseData(int mid, unsigned char* buffer, unsigned long bcount);

private:
    // 모듈 설정을 저장하기 위한 내부 구조체
    struct ModuleConfig {
        std::map<int, int> pidMap; // 채널 0-N -> 물리적 ID (PID)
        int recordLength = 0;    // FMODE용 RL
        int nChannels = 0;       // 모듈 채널 수
        int eventDataSize = 0;   // 이벤트 1개 당 바이트 크기
        ADC::MODE mode = ADC::SMODE;
        int ndp = 0; // FMODE용 샘플 수 (계산된 값)
    };

    // 등록된 모듈 설정 (MID 기준)
    std::map<int, ModuleConfig> m_moduleConfigs;

    /**
     * @brief FADC (FMODE) 원시 데이터 청크를 파싱합니다.
     * (CUPDAQ/OnlObjs/src/FADCRawEvent.cc의 Unpack_FADC 로직)
     * @param data 1개 이벤트 데이터의 시작 포인터
     * @param config 해당 모듈의 설정값
     * @return 파싱된 이벤트 객체
     */
    ParsedEventData parseFADCEvent(unsigned char* data, const ModuleConfig& config);
    
    /**
     * @brief SADC (SMODE) 원시 데이터 청크를 파싱합니다.
     * (CUPDAQ/OnlObjs/src/SADCRawEvent.cc의 Unpack_SADC 로직)
     * @param data 1개 이벤트 데이터의 시작 포인터
     * @param config 해당 모듈의 설정값
     * @return 파싱된 이벤트 객체
     */
    ParsedEventData parseSADCEvent(unsigned char* data, const ModuleConfig& config);
};

#endif // DATAPARSER_H