add_library(screencaptureapi SHARED 
    include/ScreenCaptureApi.h
    src/ScreenCaptureApiImpl.cpp
    src/ScreenCaptureApiImpl.h
    src/handler/Handler.h
    src/handler/ConnectResultHandler.cpp
    src/handler/ConnectResultHandler.h
    src/handler/QueryScreenImageResultHandler.cpp
    src/handler/QueryScreenImageResultHandler.h
    src/handler/SubscriptionHandler.cpp
    src/handler/SubscriptionHandler.h
)

target_link_libraries(screencaptureapi
    messagehelper 
    base64
    zmqpp-static 
    zmqppHeader 
    nlohmann_json)

target_include_directories(screencaptureapi PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src)

# generate export header 
include(GenerateExportHeader)
generate_export_header(screencaptureapi
    BASE_NAME ScreenCaptureApi  
    EXPORT_FILE_NAME  ${CMAKE_CURRENT_SOURCE_DIR}/include/screencaptureapi_export.h)

add_subdirectory(demo)

install(TARGETS screencaptureapi
        RUNTIME DESTINATION ${ScreenCaptureService_INSTALL_DIR}/bin
        ARCHIVE DESTINATION ${ScreenCaptureService_INSTALL_DIR}/lib)