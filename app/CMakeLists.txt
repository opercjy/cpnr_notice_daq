# app/CMakeLists.txt
set(APP_NAME cpnrcmd)
message(STATUS "Configuring ${APP_NAME} (Command Line Interface)...")

add_executable(${APP_NAME}
    cpnrcmd.cc
)

# --- 의존성 설정 ---
# CPNRCore 라이브러리가 먼저 빌드되어야 함
add_dependencies(${APP_NAME} CPNRCore)

# --- 링크 라이브러리 ---
# CPNRCore 라이브러리만 링크하면, CPNRCore에 링크된
# CPNRHardware, ROOT, libusb 등이 자동으로 함께 링크됩니다.
target_link_libraries(${APP_NAME} PRIVATE
    CPNRCore
)

# --- 설치 ---
install(TARGETS ${APP_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)