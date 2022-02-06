add_executable(ScreenCaptureServer
    main.cpp
)

target_link_libraries(ScreenCaptureServer
	screencapture
    zmqpp-static zmqppHeader nlohmann_json ws2_32)

install(TARGETS ScreenCaptureServer
        RUNTIME DESTINATION ${ScreenCaptureService_INSTALL_DIR}/bin)