// src/CPNRController.h
#ifndef CPNRCONTROLLER_H
#define CPNRCONTROLLER_H

#include <string>
#include <vector>
#include <map>
#include <thread>
#include <atomic>
#include <mutex>
#include <iostream> 
#include <iomanip>  

// CPNR 고유 상수
#include "CPNRDefs.h"
// CPNR 데이터 모델 (TTree 저장용)
#include "CPNRDataModel.h"
// CPNR 데이터 파서
#include "DataParser.h"

// ROOT 필수 헤더
#include "TFile.h"
#include "TTree.h"
#include "TError.h" // ROOT 로깅 (R__LOG)

// libusb (nkusb_init을 위해)
// "nkusb.h"는 CPNRHardware 라이브러리의 include 경로를 통해 참조됩니다.
#include "nkusb.h"

// CPNR 하드웨어 라이브러리 (ROOT 래퍼)
#include "NoticeTCBROOT.h"
#include "NoticeNKFADC500ROOT.h"
#include "NoticeM64ADCROOT.h"

/**
 * @struct CPNRModuleInfo
 * @brief TCB에 연결된 모듈 정보 (자동 탐색 결과 저장용)
 */
struct CPNRModuleInfo {
    int mid;            // 모듈 ID (예: 1, 65)
    std::string type;   // "FADC" 또는 "SADC"
    ADC::MODE mode;     // FMODE 또는 SMODE
    int nChannels;      // 채널 수 (예: 4, 32)
    unsigned long bcount; // 현재 버퍼 카운트 (모니터링용)

    // 모듈 설정값 (GUI/CMD에서 설정하고 하드웨어에 적용)
    // PID 맵핑 (채널 0-3 -> PID 1-4)
    std::map<int, int> pidMap; 
    
    // FADC 설정
    int fadc_rl = 16;
    int fadc_dacOffset = 3700;
    int fadc_threshold = 85;

    // SADC 설정
    int sadc_gw = 2000;
    int sadc_psw = 256;
    int sadc_threshold = 100;

    // TCB 전역 설정 (모듈 0번에만 저장)
    int tcb_cw = 1000;
    int tcb_ptrig = 0;

    CPNRModuleInfo() : mid(0), mode(ADC::SMODE), nChannels(0), bcount(0) {}
};

/**
 * @class CPNRController
 * @brief CPNR DAQ 시스템의 핵심 제어 클래스.
 * 하드웨어 연결, 설정, 런 제어, 데이터 읽기 및 저장을 총괄합니다.
 * (CUPDAQ의 CupDAQManager와 CupGeneralTCB의 역할을 통합)
 */
class CPNRController {
public:
    CPNRController();
    ~CPNRController();

    // 복사 및 대입 방지
    CPNRController(const CPNRController&) = delete;
    CPNRController& operator=(const CPNRController&) = delete;

    /**
     * @brief DAQ 시스템에 연결합니다.
     * @param configFilePath 설정 파일 경로 (독립형 모드 여부 등을 읽음)
     * @return 연결 성공 시 true
     */
    bool connect(const std::string& configFilePath);

    /**
     * @brief DAQ 시스템 연결을 해제합니다.
     */
    void disconnect();

    /**
     * @brief 현재 연결된 장비 상태를 터미널에 출력합니다.
     */
    void printStatus();

    /**
     * @brief FADC 모듈의 파라미터를 설정합니다. (설정값은 내부에 저장)
     * @param mid 설정할 모듈의 MID
     * @param rl Record Length (파형 길이)
     * @param dacOffset DAC 오프셋
     * @param threshold 트리거 임계값
     */
    void setFADCParams(int mid, int rl, int dacOffset, int threshold);

    /**
     * @brief SADC 모듈의 파라미터를 설정합니다. (설정값은 내부에 저장)
     * @param mid 설정할 모듈의 MID
     * @param gw Gate Width
     * @param psw Peak Sum Width
     * @param threshold 트리거 임계값
     */
    void setSADCParams(int mid, int gw, int psw, int threshold);

    /**
     * @brief TCB의 전역 설정을 구성합니다. (설정값은 내부에 저장)
     * @param cw TCB의 Coincidence Window
     * @param ptrig Pedestal Trigger 간격 (ms, 0은 비활성화)
     */
    void setTCBParams(int cw, int ptrig);

    /**
     * @brief 내부에 저장된 설정값들을 실제 하드웨어에 적용합니다.
     * @return 적용 성공 시 true
     */
    bool applyConfiguration();

    /**
     * @brief 런을 시작합니다.
     * @param runNumber 런 번호 (출력 파일명에 사용됨)
     * @return 성공 시 true
     */
    bool startRun(int runNumber);

    /**
     * @brief 런을 중지합니다.
     */
    void stopRun();

    /**
     * @brief 현재 런이 진행 중인지 확인합니다.
     * @return 런 진행 중이면 true
     */
    bool isRunning() const { return m_isRunning; }

    /**
     * @brief 설정 파일에서 DAQ 모드를 읽어옵니다.
     * @param configFilePath 설정 파일 경로
     * @return 성공 시 true
     */
    bool loadConfig(const std::string& configFilePath);

private:
    /**
     * @brief (TCB 모드) TCB에 연결된 모듈을 자동 탐색하여 m_modules에 채웁니다.
     */
    bool scanModules();

    /**
     * @brief (독립형 모드) 설정 파일 기반으로 장비를 연결합니다.
     */
    bool connectStandalone();

    /**
     * @brief 런이 시작되면 생성되는 데이터 읽기 스레드 함수입니다.
     */
    void readoutLoop();

    /**
     * @brief TCB 모드일 때, 특정 모듈에서 읽은 데이터를 파싱하고 저장합니다.
     * @param mod 데이터를 읽은 모듈 정보
     * @param buffer 데이터 버퍼 포인터
     * @param bcount 버퍼 크기 (kbytes)
     */
    void processTCBData(CPNRModuleInfo& mod, unsigned char* buffer, unsigned long bcount);

    /**
     * @brief TTree와 브랜치를 초기화합니다.
     * @return 성공 시 true
     */
    bool initializeOutput();

    // --- 상태 변수 ---
    std::atomic<bool> m_isRunning;      // 런 상태 플래그
    std::thread m_readoutThread;      // 데이터 읽기 스레드
    std::string m_configFilePath;
    std::string m_outputDirectory;
    long m_eventCounter; // 이벤트 카운터

    // --- 설정 ---
    bool m_isStandalone; // false: TCB 모드 (기본값), true: 독립형 모드
    int m_tcbSID;
    std::vector<int> m_standaloneFADC_SIDs;
    std::vector<int> m_standaloneSADC_SIDs;

    // --- 하드웨어 핸들 ---
    libusb_context* m_ctx; // libusb 세션
    NKTCB* m_tcb; // TCB 모드 핸들 (중앙 제어기)
    
    // TCB 모드에서 탐색된 모듈 목록 (MID를 키로 사용)
    std::map<int, CPNRModuleInfo> m_modules; 

    // 독립형(Standalone) 모드 핸들 (SID를 키로 사용)
    std::map<int, NKNKFADC500*> m_standaloneFADCs; 
    std::map<int, NKM64ADC*> m_standaloneSADCs;    

    // --- 데이터 파서 ---
    DataParser m_parser; // 데이터 해석 담당

    // --- ROOT 파일 출력 ---
    std::mutex m_fileMutex;     // TTree Fill 스레드 동기화용
    TFile* m_rootFile;          // 출력 ROOT 파일
    TTree* m_fadcTree;          // FMODE 데이터 TTree
    TTree* m_sadcTree;          // SMODE 데이터 TTree
    
    // TTree 브랜치에 연결될 객체들 (재사용)
    EventInfo m_eventInfo;      // 이벤트 헤더
    FChannelData m_fadcData;    // FMODE (파형) 데이터
    AChannelData m_sadcData;    // SMODE (피크) 데이터
    
    // TTree 브랜치 포인터 (TTree::Branch 호출용)
    EventInfo* m_eventInfoPtr;
    FChannelData* m_fadcDataPtr;
    AChannelData* m_sadcDataPtr;
};

#endif // CPNRCONTROLLER_H