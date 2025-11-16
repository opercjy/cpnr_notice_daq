# CPNR/hardware/CMakeLists.txt
set(MODULE_NAME CPNRHardware)
message(STATUS "Configuring ${MODULE_NAME} (Hardware Abstraction Layer)...")

# --- 1. C 소스 파일 (Layer 1, 2, 3) ---
file(GLOB C_SOURCES
    "nkusb/*.c"
    "usb3com/*.c"
    "usb3tcb/*.c"
    "NoticeTCB/*.c"
    "NoticeNKFADC500/*.c"
    "NoticeM64ADC/*.c"
)

# --- 2. C++ ROOT 래퍼 소스 (Layer 4) ---
file(GLOB CPP_SOURCES
    "ROOT_Wrappers/*.cc"
)

# --- 3. ROOT 래퍼 헤더 ---
file(GLOB ROOT_WRAPPER_HEADERS
    "ROOT_Wrappers/NoticeTCBROOT.h"
    "ROOT_Wrappers/NoticeNKFADC500ROOT.h"
    "ROOT_Wrappers/NoticeM64ADCROOT.h"
)

# --- 4. ROOT 사전 생성 ---
# ROOT_Wrappers/LinkDef.h를 사용
ROOT_GENERATE_DICTIONARY(G__${MODULE_NAME}
    ${ROOT_WRAPPER_HEADERS}
    ROOT_Wrappers/LinkDef.h
)

# --- 5. 라이브러리 빌드 ---
# C 소스, C++ 소스, 생성된 사전을 합쳐 라이브러리 생성
add_library(${MODULE_NAME} SHARED
    ${C_SOURCES}
    ${CPP_SOURCES}
    G__${MODULE_NAME}.cxx
)

# --- 6. 포함 경로 설정 ---
# 이 라이브러리를 사용하는 CPNRCore 등이 nkusb.h, NoticeTCBROOT.h 등을
# 찾을 수 있도록 INTERFACE로 경로를 공개합니다.
target_include_directories(${MODULE_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/nkusb>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/usb3com>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/usb3tcb>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/NoticeTCB>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/NoticeNKFADC500>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/NoticeM64ADC>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ROOT_Wrappers>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> # 설치 시
    ${ROOT_INCLUDE_DIRS}
)

# --- 7. 링크 라이브러리 ---
# 이 라이브러리는 ROOT와 libusb에 의존합니다.
target_link_libraries(${MODULE_NAME} PUBLIC
    ${ROOT_LIBRARIES}
    PkgConfig::LIBUSB
)

# --- 8. 설치 (라이브러리 및 헤더 파일) ---
install(TARGETS ${MODULE_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 모든 .h 헤더 파일 설치
install(DIRECTORY nkusb/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/nkusb FILES_MATCHING PATTERN "*.h")
install(DIRECTORY usb3com/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/usb3com FILES_MATCHING PATTERN "*.h")
install(DIRECTORY usb3tcb/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/usb3tcb FILES_MATCHING PATTERN "*.h")
install(DIRECTORY NoticeTCB/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/NoticeTCB FILES_MATCHING PATTERN "*.h")
install(DIRECTORY NoticeNKFADC500/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/NoticeNKFADC500 FILES_MATCHING PATTERN "*.h")
install(DIRECTORY NoticeM64ADC/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/NoticeM64ADC FILES_MATCHING PATTERN "*.h")
install(DIRECTORY ROOT_Wrappers/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ROOT_Wrappers FILES_MATCHING PATTERN "*.h")

# ROOT 사전 파일 설치
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/lib${MODULE_NAME}_rdict.pcm
    ${CMAKE_CURRENT_BINARY_DIR}/lib${MODULE_NAME}.rootmap
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    OPTIONAL
)