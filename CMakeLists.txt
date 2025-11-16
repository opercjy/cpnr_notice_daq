# CPNR/CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(CPNR LANGUAGES CXX C) # C언어(Layer 1-3) 빌드를 위해 C 추가

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# --- CERN ROOT 찾기 ---
find_package(ROOT REQUIRED COMPONENTS Core RIO Tree Graf)
include(${ROOT_USE_FILE})
message(STATUS "ROOT: ${ROOT_LIBRARIES}")

# --- libusb 찾기 ---
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB REQUIRED IMPORTED_TARGET libusb-1.0)
message(STATUS "libusb-1.0: ${LIBUSB_LIBRARIES}")

# --- 설치 경로 설정 ---
set(CMAKE_INSTALL_BINDIR bin)
set(CMAKE_INSTALL_LIBDIR lib)
set(CMAKE_INSTALL_INCLUDEDIR include)

# --- 모듈 빌드 순서 ---
# 1. 하드웨어 라이브러리 (libCPNRHardware.so)
add_subdirectory(hardware)

# 2. CPNR 핵심 로직 (libCPNRCore.so) - hardware에 의존
add_subdirectory(src)

# 3. 실행 파일 (cpnrcmd) - CPNRCore에 의존
add_subdirectory(app)