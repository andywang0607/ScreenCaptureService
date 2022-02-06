cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
set(VCPKG_ROOT "D:/ThirdParty/vcpkg/")
set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}scripts/buildsystems/vcpkg.cmake)

project ("ScreenCaptureService")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/thirdparty/cmakemodules/)
set(ScreenCaptureService_INSTALL_DIR ${CMAKE_SOURCE_DIR})
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})

set(OpenCV_DIR ${VCPKG_ROOT}installed/x64-windows/share/opencv/)
find_package(OpenCV CONFIG REQUIRED)

add_subdirectory(thirdparty)
add_subdirectory(util)

add_executable(ScreenCaptureService main.cpp)

target_link_libraries(ScreenCaptureService
	screencapture)

install(TARGETS ScreenCaptureService
        RUNTIME DESTINATION ${ScreenCaptureService_INSTALL_DIR}/bin)