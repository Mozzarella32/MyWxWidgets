#pragma once

// #define c
// #include <glad/gl.h>
// #include <GL/glew.h>

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL/glew.h>
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
#include <Utilities.hpp>
