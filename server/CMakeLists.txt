add_executable(ScreenCaptureServer
    src/ScreenCaptureServer.hpp
    src/ScreenCaptureServer.cpp
    main.cpp
)

target_link_libraries(ScreenCaptureServer
	screencapture
    base64
    messagehelper 
    zmqpp-static zmqppHeader nlohmann_json)

install(TARGETS ScreenCaptureServer
        RUNTIME DESTINATION ${ScreenCaptureService_INSTALL_DIR}/bin)