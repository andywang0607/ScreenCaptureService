add_library(messagehelper INTERFACE)
target_sources(messagehelper INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/MessageHelper.hpp
        )

target_include_directories(messagehelper INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(messagehelper INTERFACE nlohmann_json)