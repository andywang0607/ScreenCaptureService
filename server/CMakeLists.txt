add_executable(ScreenCaptureServer
    src/ScreenCaptureServer.hpp
    src/ScreenCaptureServer.cpp
    src/handler/Handler.h
    src/handler/ConnectHandler.h
    src/handler/ConnectHandler.cpp
    src/handler/QueryScreenImageHandler.h
    src/handler/QueryScreenImageHandler.cpp
    src/screencapture/ScreenCapture.h
    src/screencapture/ScreenCapture.cpp
    main.cpp
)

target_link_libraries(ScreenCaptureServer PUBLIC
    base64
    messagehelper 
    zmqpp-static nlohmann_json
    opencv)

target_include_directories(ScreenCaptureServer PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/handler
    ${CMAKE_CURRENT_SOURCE_DIR}/src/screencapture)

install(TARGETS ScreenCaptureServer
        RUNTIME DESTINATION ${ScreenCaptureService_INSTALL_DIR}/bin)

install(FILES config/server_config.json
        DESTINATION ${ScreenCaptureService_INSTALL_DIR}/bin)