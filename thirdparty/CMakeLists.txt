include (externalProjectWrapper)

# nlohmann_json
add_external_project(json_external json "JSON_BuildTests=OFF")
add_external_target(nlohmann_json json/include json OBJECT json_external)

# base64
add_external_project(base64_external base64)
add_external_target(base64 base64 base64 STATIC base64_external)

# md5
add_external_project(md5_external md5)
add_external_target(md5 md5 md5 STATIC md5_external)

# zmq
add_definitions(-DNOMINMAX)
set(ZEROMQ_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/libzmq/include)
set(ZEROMQ_LIBRARY_SHARED ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/libzmq-mt-gd-4_2_3.lib)
set(ZEROMQ_LIBRARY_STATIC ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt/libzmq-static.lib)
set(ZEROMQ_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/prebuilt)

execute_process(COMMAND apply-zmqpp-patch.bat
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/patch)
add_subdirectory(zmqpp)

add_library(zmqppHeader INTERFACE)
target_include_directories(zmqppHeader SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/zmqpp/src ${CMAKE_CURRENT_BINARY_DIR}/zmqpp ${CMAKE_CURRENT_SOURCE_DIR}/libzmq/include)

# opencv
add_library(opencv INTERFACE)
target_include_directories(opencv INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/opencv/include)

if(NOT ${CMAKE_BUILD_TYPE} MATCHES "Debug")
        target_link_libraries(opencv INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/opencv/lib/opencv_world349.lib)
        install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/opencv/bin/opencv_world349.dll
                DESTINATION ${ScreenCaptureService_INSTALL_DIR}/bin)
else()
        target_link_libraries(opencv INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/opencv/lib/opencv_world349d.lib)
        install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/opencv/bin/opencv_world349d.dll
                DESTINATION ${ScreenCaptureService_INSTALL_DIR}/bin)
endif()