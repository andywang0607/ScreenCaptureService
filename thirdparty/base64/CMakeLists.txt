add_library(base64 STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/base64.h
        ${CMAKE_CURRENT_SOURCE_DIR}/base64.cpp
)

target_include_directories(base64 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})