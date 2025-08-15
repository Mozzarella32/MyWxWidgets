cmake_minimum_required(VERSION 3.13)

project(MyWxWidgets)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(${PROJECT_SOURCE_DIR}/submodules/MyOpenGl)

# Directories
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(LIB_DIR "${PROJECT_SOURCE_DIR}/lib")

set(PCH_HEADERS "${INCLUDE_DIR}/pch.hpp")

file(GLOB_RECURSE MY_SOURCE CONFIGURE_DEPENDS "${SRC_DIR}/*")

file(GLOB_RECURSE MY_HEADERS CONFIGURE_DEPENDS "${INCLUDE_DIR}/*")

source_group("Source Files" FILES ${MY_SOURCE})
source_group("Header Files" FILES ${MY_HEADERS})

list(REMOVE_ITEM MY_SOURCE ${PCH_HEADERS})
list(REMOVE_ITEM MY_HEADERS ${PCH_HEADERS})

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Werror -Wall -Wextra -Wpedantic -Wno-missing-field-initializers)
endif()

# Statische Bibliothek erstellen
add_library(${PROJECT_NAME} STATIC ${MY_SOURCE})

# Precompiled Header (falls verwendet)
target_precompile_headers(${PROJECT_NAME} PRIVATE ${PCH_HEADERS})

# Include-Direktiven
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR})

# MyOpenGl Linking
target_link_libraries(${PROJECT_NAME} PUBLIC MyOpenGl wx::gl)
