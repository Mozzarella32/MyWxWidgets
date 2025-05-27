cmake_minimum_required(VERSION 3.13)

project(MyWxWidgets)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# WxWidgets
find_package(wxWidgets QUIET)

if (NOT wxWidgets_FOUND)
    message(STATUS "wxWidgets not found")
    set(wxBUILD_SHARED OFF)
    set(wxBUILD_TESTS OFF)
    set(wxBUILD_DEMOS OFF)
    set(wxBUILD_EXAMPLES OFF)

    message(STATUS "Fetching wxWidgets...")
    FetchContent_Declare(
       wxWidgets
       GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets.git
       GIT_SHALLOW ON
       CMAKE_ARGS
           -DwxUSE_STD=0
    )
    FetchContent_MakeAvailable(wxWidgets)
else()
    message(STATUS "wxWidgets Found")
endif()

# GLEW
find_package(GLEW QUIET)

if (NOT GLEW_FOUND)
    message(STATUS "GLEW not found")
    message(STATUS "Fetching GLEW...")

    set(glew-cmake_BUILD_SHARED OFF)
    set(USE_GLU ON)
    # https://github.com/Perlmint/glew-cmake/blob/master/glew-cmake/fetch-content.cmake
    FetchContent_Declare(
       GLEW
       GIT_REPOSITORY https://github.com/Perlmint/glew-cmake
       GIT_SHALLOW ON
    )

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DGLEW_EGL")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGLEW_EGL")
        
    FetchContent_MakeAvailable(glew)

    #add_subdirectory(${glew_SOURCE_DIR} ${glew_BINARY_DIR} EXCLUDE_FROM_ALL)
else()
    message(STATUS "GLEW Found")
endif()

# STB - Header-only-Bibliothek
message(STATUS "Fetching stb...")

FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_SHALLOW ON
)

FetchContent_MakeAvailable(stb)

set(STB_INCLUDE_DIR ${stb_SOURCE_DIR})

# MyOpenGl
message(STATUS "Fetching MyOpenGl...")

FetchContent_Declare(
       MyOpenGl
       GIT_REPOSITORY https://github.com/Mozzarella32/MyOpenGl.git
       GIT_SHALLOW ON
       GIT_TAG main
)
FetchContent_MakeAvailable(MyOpenGl)

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

# Statische Bibliothek erstellen
add_library(${PROJECT_NAME} STATIC ${MY_SOURCE})

# Precompiled Header (falls verwendet)
target_precompile_headers(${PROJECT_NAME} PRIVATE ${PCH_HEADERS})

# Include-Direktiven
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIR} ${STB_INCLUDE_DIR})

# wxWidgets Linking
target_link_libraries(${PROJECT_NAME} PUBLIC wx::net wx::core wx::base)

# GLEW Linking
 if (NOT GLEW_FOUND)
   target_link_libraries(${PROJECT_NAME} PRIVATE libglew_static)
 else()
   target_link_libraries(${PROJECT_NAME} PUBLIC GLEW::glew_s)
 endif()

# MyOpenGl Linking
target_link_libraries(${PROJECT_NAME} PUBLIC MyOpenGl)
