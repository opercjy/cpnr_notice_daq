// app/cpnrcmd.cc
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>      // 파일 존재 확인
#include <stdexcept>    // std::stoi
#include "CPNRController.h" // (src/CPNRController.h)
#include "TSystem.h"    // gSystem (ROOT)

/**
 * @brief CPNR DAQ 제어용 CMD 라인 인터페이스
 * * 이 프로그램은 CPNRController 클래스를 래핑하여,
 * 사용자가 커맨드를 입력해 DAQ를 제어할 수 있도록 합니다.
 */
void print_usage() {
    std::cout << "\n===== CPNR DAQ Command Line Interface =====" << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "  config <file>  - DAQ 모드 설정 (예: config config/cpnr.config)" << std::endl;
    std::cout << "  connect        - 설정 파일에 따라 하드웨어 연결 및 자동 탐색" << std::endl;
    std::cout << "  status         - 현재 연결된 장비 상태 및 구성 확인" << std::endl;
    std::cout << "  configure      - (테스트용) 하드코딩된 값으로 장비 설정 적용" << std::endl;
    std::cout << "  start <run_num>  - 런 시작 (예: start 123)" << std::endl;
    std::cout << "  stop           - 런 중지" << std::endl;
    std::cout << "  exit           - 프로그램 종료 (런 자동 중지)" << std::endl;
    std::cout << "===========================================" << std::endl;
}

// 파일 존재 여부 확인 헬퍼 함수
bool fileExists(const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

int main(int argc, char* argv[]) {
    
    // ROOT TApplication을 생성하여 ROOT 라이브러리 및 시그널 핸들러 초기화
    // GUI가 없으므로 new TApplication("cpnrcmd", &argc, argv, 0, -1)의
    // 경량화 버전인 gSystem->ProcessEvents() 등을 사용할 수 있으나,
    // 여기서는 CPNRController가 ROOT 객체를 직접 소유하므로
    // ROOT의 전역 상태 초기화가 필요할 수 있습니다. (LinkDef 로딩 등)
    // TApplication app("cpnrcmd", &argc, argv); // (GUI가 없으므로 주석 처리)
    // gSystem->Load("libCPNRHardware.so"); // (CMake가 링크하므로 불필요)
    // gSystem->Load("libCPNRCore.so");     // (CMake가 링크하므로 불필요)

    CPNRController daq;
    std::string line;
    
    print_usage();

    bool isConnected = false;

    while (true) {
        std::cout << "\nCPNR > " << std::flush;
        if (!std::getline(std::cin, line)) {
            // Ctrl+D (EOF) 처리
            line = "exit";
            std::cout << "exit" << std::endl;
        }

        std::istringstream iss(line);
        std::string command;
        iss >> command;

        if (command.empty()) continue;

        if (command == "config") {
            if (daq.isRunning()) {
                std::cerr << "  [오류] 런(Run)이 진행 중일 때는 설정을 변경할 수 없습니다." << std::endl;
                continue;
            }
            std::string configFile;
            iss >> configFile;
            if (configFile.empty()) {
                std::cerr << "  [오류] 설정 파일 경로가 필요합니다. (예: config config/cpnr.config)" << std::endl;
            } else if (!fileExists(configFile)) {
                std::cerr << "  [오류] 설정 파일을 찾을 수 없습니다: " << configFile << std::endl;
            } else {
                if (!daq.loadConfig(configFile)) {
                    std::cerr << "  [오류] 설정 파일 로드 실패: " << configFile << std::endl;
                } else {
                    std::cout << "  [성공] 설정 로드: " << configFile << std::endl;
                }
            }
        } 
        else if (command == "connect") {
            if (daq.isRunning()) {
                std::cerr << "  [오류] 런(Run)이 진행 중일 때는 연결을 변경할 수 없습니다." << std::endl;
                continue;
            }
            if (isConnected) {
                std::cout << "  [알림] 이미 연결되어 있습니다. 다시 연결합니다." << std::endl;
                daq.disconnect();
            }
            
            if (daq.connect("config/cpnr.config")) { // (간편화를 위해 기본값 사용)
                std::cout << "  [성공] 장비 연결 및 모듈 탐색 완료." << std::endl;
                daq.printStatus();
                isConnected = true;
            } else {
                std::cerr << "  [오류] 장비 연결에 실패했습니다. USB 연결 및 전원을 확인하십시오." << std::endl;
                isConnected = false;
            }
        }
        else if (command == "status") {
            daq.printStatus();
        }
        else if (command == "configure") {
            if (!isConnected) {
                std::cerr << "  [오류] 'connect' 명령을 통해 먼저 장비에 연결해야 합니다." << std::endl;
                continue;
            }
            // TCB 모드 및 현재 구성(FADC 1, SADC 1)을 가정한 하드코딩된 설정 예시
            std::cout << "  [알림] TCB 및 탐색된 모듈을 기본값으로 설정합니다..." << std::endl;
            // TCB 전역 설정
            daq.setTCBParams(1000, 0); // CW=1000ns, Pedestal Trig Off
            // FADC (MID 1 가정) 설정
            daq.setFADCParams(1, 16, 3700, 85); // MID=1, RL=16, Offset=3700, Thr=85
            // SADC (MID 65 가정) 설정
            daq.setSADCParams(65, 2000, 256, 100); // MID=65, GW=2000, PSW=256, Thr=100
            
            if (daq.applyConfiguration()) {
                std::cout << "  [성공] 하드웨어 설정이 적용되었습니다." << std::endl;
            } else {
                std::cerr << "  [오류] 하드웨어 설정 적용에 실패했습니다." << std::endl;
            }
        }
        else if (command == "start") {
            if (!isConnected) {
                std::cerr << "  [오류] 'connect' 명령을 통해 먼저 장비에 연결해야 합니다." << std::endl;
                continue;
            }
            if (daq.isRunning()) {
                std::cerr << "  [오류] 이미 런이 진행 중입니다." << std::endl;
                continue;
            }

            int runNum = 0;
            std::string runNumStr;
            iss >> runNumStr;
            try {
                if (!runNumStr.empty()) runNum = std::stoi(runNumStr);
            } catch (const std::exception&) { runNum = 0; }

            if (runNum <= 0) {
                std::cerr << "  [오류] 유효한 런 번호가 필요합니다. (예: start 123)" << std::endl;
            } else {
                if (daq.startRun(runNum)) {
                    std::cout << "  [성공] 런 " << runNum << " 시작됨. 데이터 읽기 스레드 동작 중..." << std::endl;
                    std::cout << "  (런을 중지하려면 'stop'을 입력하십시오)" << std::endl;
                } else {
                    std::cerr << "  [오류] 런 시작에 실패했습니다. (장비 연결 및 설정을 확인하십시오)" << std::endl;
                }
            }
        }
        else if (command == "stop") {
            if (daq.isRunning()) {
                daq.stopRun();
                std::cout << "  [성공] 런이 중지되었습니다." << std::endl;
            } else {
                std::cout << "  [알림] 현재 진행 중인 런이 없습니다." << std::endl;
            }
        }
        else if (command == "exit") {
            if (daq.isRunning()) {
                std::cout << "  [알림] 진행 중인 런을 먼저 중지합니다..." << std::endl;
                daq.stopRun();
            }
            std::cout << "  [알림] 장비 연결을 해제하고 CPNR DAQ를 종료합니다." << std::endl;
            daq.disconnect();
            break;
        }
        else {
            std::cerr << "  [오류] 알 수 없는 명령: " << command << std::endl;
            print_usage();
        }
    }

    return 0;
}