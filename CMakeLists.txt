cmake_minimum_required(VERSION 3.15)
project(ida_discord_rpc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(NOT UNIX OR APPLE)
    message(FATAL_ERROR "This project is Linux-only (Ubuntu/IDA 9.3).")
endif()

set(IDASDK_PATH "" CACHE PATH "Path to IDA SDK root (must contain include/ida.hpp)")
set(IDASDK_LIB "" CACHE FILEPATH "Path to libida.a or libida.so")
set(IDA_PLUGIN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/ida_plugins" CACHE PATH "Output directory for built plugin")

set(IDASDK_INCLUDE_DIR "${IDASDK_PATH}/include")
if(NOT EXISTS "${IDASDK_INCLUDE_DIR}/ida.hpp" AND EXISTS "${IDASDK_PATH}/src/include/ida.hpp")
    set(IDASDK_INCLUDE_DIR "${IDASDK_PATH}/src/include")
endif()

if(NOT IDASDK_PATH)
    message(FATAL_ERROR "IDASDK_PATH is not set. Example: -DIDASDK_PATH=/opt/ida-sdk-9.3")
endif()

if(NOT EXISTS "${IDASDK_INCLUDE_DIR}/ida.hpp")
    message(FATAL_ERROR
        "Invalid IDASDK_PATH='${IDASDK_PATH}'. Missing ida.hpp in "
        "'${IDASDK_PATH}/include' or '${IDASDK_PATH}/src/include'"
    )
endif()

if(NOT IDASDK_LIB)
    file(GLOB _ida_lib_candidates
        "${IDASDK_PATH}/lib/x64_linux_gcc_64*/libida.a"
        "${IDASDK_PATH}/lib/x64_linux_gcc_64*/libida.so"
        "${IDASDK_PATH}/libida.a"
        "${IDASDK_PATH}/libida.so"
        "${IDASDK_PATH}/../libida.a"
        "${IDASDK_PATH}/../libida.so"
    )

    list(LENGTH _ida_lib_candidates _ida_lib_count)
    if(_ida_lib_count EQUAL 0)
        message(FATAL_ERROR
            "Could not auto-detect IDA SDK library under '${IDASDK_PATH}/lib'. "
            "Set -DIDASDK_LIB explicitly."
        )
    endif()

    list(SORT _ida_lib_candidates)
    list(GET _ida_lib_candidates -1 IDASDK_LIB)
    message(STATUS "Auto-detected IDASDK_LIB='${IDASDK_LIB}'")
endif()

if(NOT EXISTS "${IDASDK_LIB}")
    message(FATAL_ERROR "IDASDK_LIB does not exist: '${IDASDK_LIB}'")
endif()

include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "Disable discord-rpc examples" FORCE)
include(cmake/fetch_discord_rpc.cmake) 
set_target_properties(discord-rpc PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_library(ida_discord_rpc SHARED
    src/plugin.cpp
    src/plugin_mod.cpp

    src/ida_helper.cpp
    src/discord_rpc_helper.cpp
    
    src/log/log.cpp
    src/log/log_level.cpp

    src/discord/rich_presence.cpp

    src/listener/ui_listener.cpp
    src/listener/idb_listener.cpp
    src/listener/view_listener.cpp

)

target_include_directories(ida_discord_rpc PRIVATE
    ${IDASDK_INCLUDE_DIR}
    ${discord-rpc_SOURCE_DIR}/include
    src
)

target_link_libraries(ida_discord_rpc PRIVATE
    "${IDASDK_LIB}"
    discord-rpc
)

target_compile_definitions(ida_discord_rpc PRIVATE __LINUX__ __EA64__)

set_target_properties(ida_discord_rpc PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${IDA_PLUGIN_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${IDA_PLUGIN_OUTPUT_DIR}
    PREFIX ""
    OUTPUT_NAME "ida_discord_rpc"
)
