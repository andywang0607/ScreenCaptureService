add_library(md5 STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/md5.h
        ${CMAKE_CURRENT_SOURCE_DIR}/md5.cpp
)

target_include_directories(md5 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})