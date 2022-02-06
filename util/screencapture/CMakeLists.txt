
# add_definitions(-DNOMINMAX)
add_definitions(-Dno_init_all)
add_definitions(-D_AMD64_)

add_library(screencapture STATIC 
    ScreenCapture.cpp
    ScreenCapture.h)

if( OpenCV_FOUND )
    # Additional Include Directories
    target_include_directories(screencapture PUBLIC ${OpenCV_INCLUDE_DIRS} )

    # Additional Library Directories
    target_link_libraries(screencapture PUBLIC ${OpenCV_LIBS} )
endif()

# generate export header 
include(GenerateExportHeader)
generate_export_header(screencapture 
    EXPORT_FILE_NAME  ${CMAKE_CURRENT_SOURCE_DIR}/screencapture_export.h)


install(TARGETS screencapture
        RUNTIME DESTINATION ${ScreenCaptureService_INSTALL_DIR}/bin
        ARCHIVE DESTINATION ${ScreenCaptureService_INSTALL_DIR}/lib)