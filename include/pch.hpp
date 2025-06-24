#pragma once

// #define c
// #include <glad/gl.h>
// #include <GL/glew.h>

// #ifndef GLEW_STATIC
// #define GLEW_STATIC
// #endif

#include <glad/glad.h>

#ifdef _WIN32

#ifndef _SILENCE_CXX23_DENORM_DEPRECATION_WARNING
#define _SILENCE_CXX23_DENORM_DEPRECATION_WARNING
#endif
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif

#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <GL/gl.h>
#endif

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

// #include <GL/glew.h>
// #ifdef _WIN32
    // #include <GL/GL.h>
// #else
    // #include <GL/gl.h>
// #endif

#include <wx/wx.h>
#include <wx/settings.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>
#include <wx/collpane.h>
#include <wx/glcanvas.h>

#include <string>
#include <vector>
#include <stack>
#include <filesystem>
#include <concepts>
#include <condition_variable>
#include <optional>
#include <unordered_set>

#include <Utilities.hpp>
