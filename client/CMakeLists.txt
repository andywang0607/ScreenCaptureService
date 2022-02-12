add_executable(client
    include/ScreenCaptureApi.h
    src/ScreenCaptureApiImpl.cpp
    src/ScreenCaptureApiImpl.h
    main.cpp
)

target_link_libraries(client
    messagehelper 
    base64
    threadsafeQueue
    zmqpp-static zmqppHeader nlohmann_json
    ${OpenCV_LIBS})

target_include_directories(client PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS})

install(TARGETS client
        RUNTIME DESTINATION ${ScreenCaptureService_INSTALL_DIR}/bin)