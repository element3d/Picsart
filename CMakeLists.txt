cmake_minimum_required(VERSION 3.0)

set(CMAKE_BUILD_TYPE DEBUG)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin)

message(STATUS **********************)
message(STATUS $ENV{ELEMENT3D_DIR})

set(LIBS_DIR $ENV{ELEMENT3D_DIR}/libs/lib)
set(INCLUDES_DIR $ENV{ELEMENT3D_DIR}/libs/include)

include_directories(
	${CMAKE_SOURCE_DIR}
	${INCLUDES_DIR}
	${INCLUDES_DIR}/Carbon
	${INCLUDES_DIR}/element3d
	${INCLUDES_DIR}/glm
	${INCLUDES_DIR}/glew
)
if (ANDROID)
    add_definitions(-D__E3_TARGET_ANDROID__ -D__E3_OS_ANDROID__)
    link_directories(${LIBS_DIR}/${ANDROID_ABI})
elseif(EMSCRIPTEN)
    add_definitions(-D__E3_OS_WEB__)
    link_directories(${LIBS_DIR}/emscripten)
elseif(WIN32)
    add_definitions(-D__E3_OS_WINDOWS__)
    link_directories(${LIBS_DIR}/Win32)
elseif(UNIX)
    add_definitions(-D__E3_OS_LINUX__)
    link_directories(${LIBS_DIR}/Linux)
endif()

include_directories(Picsart)
add_subdirectory(Picsart)

