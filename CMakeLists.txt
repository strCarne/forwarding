cmake_minimum_required(VERSION 3.25)

# Enabled, because neovim linter uses compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(forwarding
    VERSION 0.0.0
    LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

set(PROJECT_LIB_NAME ${PROJECT_NAME}-api)

file(GLOB PROJECT_LIB_SOURCE CONFIGURE_DEPENDS "src/*.c")
include_directories(include)

add_library(${PROJECT_LIB_NAME} ${PROJECT_LIB_SOURCE})

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME} ${PROJECT_LIB_NAME})

add_subdirectory(dotenv)
include_directories(dotenv)
target_link_libraries(${PROJECT_NAME} dotenv)

find_package(PkgConfig REQUIRED)
if (${PKG_CONFIG_FOUND})
    pkg_check_modules(DPDK "libdpdk")
    if (${DPDK_FOUND})
        message(STATUS "found dpd via pkg-config")
    else()
        message(FATAL_ERROR "couldn't find dpdk via pkg-config")
    endif()
endif()


if (${DPDK_FOUND})
    add_definitions(${DPDK_CFLAGS})
    # set(DPDK_LIBRARIES -Wl,--whole-archive ${DPDK_LIBRARIES} -lpthread -lnuma -ldl -Wl,--no-whole-archive)
    set(DPDK_LIBRARIES ${DPDK_LDFLAGS})
    include_directories(${DPDK_INCLUDE_DIRS})
    link_libraries(${DPDK_LDFLAGS})
    add_definitions(-DHAVE_DPDK)

    target_link_libraries(${PROJECT_NAME} ${DPDK_STATIC_LIBRARIES})
endif()

# if (DEFINED ENV{RTE_SDK})
#   message(STATUS "RTE_SDK=$ENV{RTE_SDK}")
# else()
#   message(FATAL_ERROR "RTE_SDK is not set! It must point into dpdk directory")
# endif()
#
# include_directories($ENV{RTE_SDK}/build/include)
# link_directories($ENV{RTE_SDK}/build/lib)
# set(DPDK_LIBS "-Wl,--whole-archive -ldpdk -Wl,--no-whole-archive")
# target_link_libraries(${PROJECT_NAME} ${DPDK_LIBS} pthread dl rt m numa)
