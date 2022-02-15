add_executable(clientDemo
    main.cpp
)

target_link_libraries(clientDemo
    screencaptureapi
    threadsafeQueue
    opencv
)

install(TARGETS clientDemo
        RUNTIME DESTINATION ${ScreenCaptureService_INSTALL_DIR}/bin)