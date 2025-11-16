# src/CMakeLists.txt
set(MODULE_NAME CPNRCore)
message(STATUS "Configuring ${MODULE_NAME} (Core Logic)...")

# --- 소스 파일 ---
file(GLOB MODULE_SOURCES
    "*.cc"
)

# --- 헤더 파일 ---
file(GLOB MODULE_HEADERS
    "*.h"
)

# --- ROOT 사전 생성 ---
# CPNRDataModel.h의 클래스들을 TTree에 저장하기 위해 사전 생성
ROOT_GENERATE_DICTIONARY(G__${MODULE_NAME}
    CPNRDataModel.h  # (D)
    LinkDef.h        # (G)
)

# --- 라이브러리 빌드 ---
add_library(${MODULE_NAME} SHARED
    ${MODULE_SOURCES}
    G__${MODULE_NAME}.cxx
)

# --- 의존성 설정 ---
# CPNRHardware 라이브러리가 먼저 빌드되어야 함
add_dependencies(${MODULE_NAME} CPNRHardware)

# --- 포함 경로 ---
target_include_directories(${MODULE_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    ${ROOT_INCLUDE_DIRS}
    # CPNRHardware에서 공개한 경로 (nkusb.h, NoticeTCBROOT.h 등을 찾기 위함)
    ${CMAKE_SOURCE_DIR}/hardware
)

# --- 링크 라이브러리 ---
# libusb, ROOT, 그리고 CPNRHardware 라이브러리
target_link_libraries(${MODULE_NAME} PUBLIC
    ${ROOT_LIBRARIES}
    PkgConfig::LIBUSB
    CPNRHardware      # (hardware/ 에서 빌드된 라이브러리)
)

# --- 설치 ---
install(TARGETS ${MODULE_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(FILES ${MODULE_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${MODULE_NAME}
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/lib${MODULE_NAME}_rdict.pcm
    ${CMAKE_CURRENT_BINARY_DIR}/lib${MODULE_NAME}.rootmap
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    OPTIONAL
)