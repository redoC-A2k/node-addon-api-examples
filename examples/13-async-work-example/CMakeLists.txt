cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)
cmake_policy(SET CMP0042 NEW)

# Name of the project (will be the name of the plugin)
project (AsyncWorkerAddon)

add_definitions(-DNAPI_VERSION=6)

message(STATUS "CMAKE_JS_INC: ${CMAKE_JS_INC}")
include_directories(${CMAKE_JS_INC})

file(GLOB SOURCE_FILES src/addon.cpp src/native.cpp)
message(STATUS "SOURCE_FILES: ${SOURCE_FILES}")
message(STATUS "CMAKE_JS_LIB: ${CMAKE_JS_SRC}")
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC})
message(STATUS "CMAKE_JS_LIB: ${CMAKE_JS_LIB}")
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})

if(MSVC AND CMAKE_JS_NODELIB_DEF AND CMAKE_JS_NODELIB_TARGET)
  # Generate node.lib
  execute_process(COMMAND ${CMAKE_AR} /def:${CMAKE_JS_NODELIB_DEF} /out:${CMAKE_JS_NODELIB_TARGET} ${CMAKE_STATIC_LINKER_FLAGS})
endif()