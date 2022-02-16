cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)

project("ScreenCaptureService")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
                      ${CMAKE_SOURCE_DIR}/thirdparty/cmakemodules/)
set(ScreenCaptureService_INSTALL_DIR ${PROJECT_SOURCE_DIR})
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

set(targets thirdparty util server client)
foreach(target ${targets})
  add_subdirectory(${target})
endforeach(target)
