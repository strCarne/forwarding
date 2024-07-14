cmake_minimum_required(VERSION 3.25)

# Enabled, because neovim linter uses compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(dotenv
    VERSION 0.0.0
    LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

file(GLOB LIB_SOURCE CONFIGURE_DEPENDS "src/*.c")
include_directories(include)

add_library(${PROJECT_NAME} SHARED ${LIB_SOURCE})

