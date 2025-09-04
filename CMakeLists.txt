cmake_minimum_required(VERSION 3.13)

project(MyWxWidgets)

include(FetchContent)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(${PROJECT_SOURCE_DIR}/submodules/wxWidgets)
add_subdirectory(${PROJECT_SOURCE_DIR}/submodules/MyOpenGl)

# Directories
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(LIB_DIR "${PROJECT_SOURCE_DIR}/lib")

set(PCH_HEADERS "${INCLUDE_DIR}/pch.hpp")

set(MY_SOURCE
    src/ChartCtrl.cpp
    src/GameLoopTimer.cpp
    src/pch.cpp
    src/ValidatorWithLabel.cpp
    src/ElementLayerer.cpp
    src/GLInitialiser.cpp
    src/RectangleMarker.cpp
    src/WxTextureUtil.cpp
    src/FunktionEvent.cpp
    src/MarkupString.cpp
    src/Utility.cpp
)

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
