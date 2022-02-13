add_executable(clientDemo
    main.cpp
)

target_link_libraries(clientDemo
    screencaptureapi
    threadsafeQueue
    ${OpenCV_LIBS})

target_include_directories(clientDemo INTERFACE
    ${OpenCV_INCLUDE_DIRS})

install(TARGETS clientDemo
        RUNTIME DESTINATION ${ScreenCaptureService_INSTALL_DIR}/bin)