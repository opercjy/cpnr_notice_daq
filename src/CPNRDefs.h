// src/CPNRDefs.h
#ifndef CPNRDEFS_H
#define CPNRDEFS_H

/**
 * @file CPNRDefs.h
 * @brief CPNR 프로젝트 전용 상수 정의 헤더.
 * CPNR 프로젝트에 필요한 상수(ADC 타입, 하드웨어 ID 등)만 캡슐화하여 정의합니다.
 * (CUPDAQ의 OnlConsts 모듈을 대체)
 */

#include <string>

// --- 데이터 크기 상수 ---
// (CUPDAQ/OnlConsts/adcconsts.hh에서 차용)
const unsigned long kKILOBYTES = 1024;
const unsigned long kMEGABYTES = kKILOBYTES * kKILOBYTES;
const unsigned long kGIGABYTES = kKILOBYTES * kKILOBYTES * kKILOBYTES;
const double kDGIGABYTES = 1024. * 1024. * 1024.;

// --- ADC 모드 (FMODE/SMODE) ---
// (CUPDAQ/OnlConsts/adcconsts.hh에서 차용)
namespace ADC {
/**
 * @enum MODE
 * @brief ADC 데이터 수집 모드
 */
enum MODE {
    FMODE = 1, /**< Waveform (Flash) Mode */
    SMODE = 2  /**< Peak/Charge (Scalar) Mode */
};
} // namespace ADC

// --- TCB 타입 ---
// (CUPDAQ/OnlConsts/adcconsts.hh에서 차용)
namespace TCB {
/**
 * @enum TYPE
 * @brief TCB 하드웨어 버전
 */
enum TYPE {
    V1 = 0,   /**< TCB v1 (Full-size) */
    MINI = 1, /**< Mini TCB (Ethernet) */
    V2 = 2    /**< TCB v2 (Full-size,
                   멀티 ADC 타입 지원) */
};
} // namespace TCB

// --- 트리거 모드 ---
// (CUPDAQ/OnlConsts/adcconsts.hh에서 차용)
namespace TRIGGER {
/**
 * @enum MODE
 * @brief DAQ 트리거 모드
 */
enum MODE {
    SELF = 1,   /**< ADC 모듈 자체 트리거 */
    MODULE = 2, /**< (미사용) */
    GLOBAL = 3  /**< TCB의 전역 트리거 동기화 */
};
} // namespace TRIGGER

// --- 하드웨어 벤더 및 제품 ID ---
// (NoticeTCB.h, NoticeNKFADC500.h, NoticeM64ADC.h에서 차용)

/** @brief Anchor/Cypress Vendor ID */
const uint16_t TCB_VENDOR_ID = 0x0547;
/** @brief TCB (Trigger Control Board) Product ID */
const uint16_t TCB_PRODUCT_ID = 0x1501;

/** @brief Anchor/Cypress Vendor ID */
const uint16_t NKFADC500_VENDOR_ID = 0x0547;
/** @brief NKFADC500 (FADC) Product ID */
const uint16_t NKFADC500_PRODUCT_ID = 0x1502;

/** @brief Anchor/Cypress Vendor ID */
const uint16_t M64ADC_VENDOR_ID = 0x0547;
/** @brief M64ADC (SADC) Product ID */
const uint16_t M64ADC_PRODUCT_ID = 0x1503;


// --- 채널 및 데이터 크기 상수 ---
// (CUPDAQ/OnlConsts/adcconsts.hh에서 차용)

/** @brief SADC (M64ADC)의 채널 수 */
const int kNCHSADC = 32;
/** @brief SADC (SMODE) 이벤트의 고정 바이트 크기 */
const unsigned long kBYTESPEREVENTSADC = 256;

/** @brief FADC (NKFADC500)의 채널 수 */
const int kNCHFADC = 4;

/** @brief TCB 메모리 맵에서 FADC 타입 식별 비트 (MID[7:6] == 0b00) */
const unsigned char kFADC_MID_TYPE = 0b00;
/** @brief TCB 메모리 맵에서 SADC 타입 식별 비트 (MID[7:6] == 0b01) */
const unsigned char kSADC_MID_TYPE = 0b01;

#endif // CPNRDEFS_H