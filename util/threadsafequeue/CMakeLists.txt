add_library(threadsafeQueue INTERFACE)
target_sources(threadsafeQueue
               INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/threadsafe_queue.hpp)

target_include_directories(threadsafeQueue
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
