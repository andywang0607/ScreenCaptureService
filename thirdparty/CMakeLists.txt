include(externalProjectWrapper)

# nlohmann_json
add_external_project(json_external json "JSON_BuildTests=OFF")
add_external_target(nlohmann_json json/include json OBJECT json_external)

# base64
add_external_project(base64_external base64)
add_external_target(base64 base64 base64 STATIC base64_external)

# md5
add_external_project(md5_external md5)
add_external_target(md5 md5 md5 STATIC md5_external)

# spdlog
add_external_project(spdlog_external spdlog)
add_external_target(spdlog spdlog/include spdlog OBJECT spdlog_external)

# zmq
add_external_project(zmq_external libzmq "BUILD_TESTS=off")
add_external_target(libzmq-static libzmq/include libzmq/lib STATIC zmq_external)
add_external_target(libzmq libzmq/include libzmq/lib SHARED zmq_external)
install(
  DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/libzmq/lib/
  DESTINATION lib
  FILES_MATCHING
  PATTERN "libzmq*")

add_definitions(-DNOMINMAX)
set(ZEROMQ_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/libzmq/include)
if(NOT ${CMAKE_BUILD_TYPE} MATCHES "Debug")
  set(ZEROMQ_LIBRARY_SHARED
      ${CMAKE_CURRENT_BINARY_DIR}/libzmq/lib/${CMAKE_BUILD_TYPE}/libzmq-v141-mt-4_3_2.lib
  )
  set(ZEROMQ_LIBRARY_STATIC
      ${CMAKE_CURRENT_BINARY_DIR}/libzmq/lib/${CMAKE_BUILD_TYPE}/libzmq-v141-mt-s-4_3_2.lib
  )
else()
  set(ZEROMQ_LIBRARY_SHARED
      ${CMAKE_CURRENT_BINARY_DIR}/libzmq/lib/${CMAKE_BUILD_TYPE}/libzmq-v141-mt-gd-4_3_2.lib
  )
  set(ZEROMQ_LIBRARY_STATIC
      ${CMAKE_CURRENT_BINARY_DIR}/libzmq/lib/${CMAKE_BUILD_TYPE}/libzmq-v141-mt-sgd-4_3_2.lib
  )
endif()
install(
  DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/zmqpp/
  DESTINATION lib
  FILES_MATCHING
  PATTERN "libzmqpp*")

# zmqpp
execute_process(COMMAND apply-zmqpp-patch.bat
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/patch)
add_subdirectory(zmqpp)
target_include_directories(
  zmqpp-static
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/zmqpp/src
         ${CMAKE_CURRENT_BINARY_DIR}/zmqpp
         ${CMAKE_CURRENT_SOURCE_DIR}/libzmq/include)

# opencv
add_library(opencv INTERFACE)
target_include_directories(opencv
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/opencv/include)

if(NOT ${CMAKE_BUILD_TYPE} MATCHES "Debug")
  target_link_libraries(
    opencv INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/opencv/lib/opencv_world349.lib)
  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/opencv/bin/opencv_world349.dll
          DESTINATION ${ScreenCaptureService_INSTALL_DIR}/bin)
else()
  target_link_libraries(
    opencv
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/opencv/lib/opencv_world349d.lib)
  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/opencv/bin/opencv_world349d.dll
          DESTINATION ${ScreenCaptureService_INSTALL_DIR}/bin)
endif()
