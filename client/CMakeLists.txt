add_executable(client
    src/ScreenCaptureApi.cpp
    src/ScreenCaptureApi.h
    main.cpp
)

target_link_libraries(client
    messagehelper 
    zmqpp-static zmqppHeader nlohmann_json)

install(TARGETS client
        RUNTIME DESTINATION ${ScreenCaptureService_INSTALL_DIR}/bin)