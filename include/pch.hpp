#pragma once

#include <glad/glad.h>

#ifdef _WIN32

#undef APIENTRY

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

#include <winsock2.h>
#include <Windows.h>
#include <GL/gl.h>
#endif

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include <wx/settings.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>
#include <wx/collpane.h>
#include <wx/msgdlg.h>
#include <wx/app.h>
#include <wx/textentry.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/frame.h>
#include <wx/validate.h>
#include <wx/sizer.h>

#include <string>
#include <vector>
#include <stack>
#include <filesystem>
#include <concepts>
#include <condition_variable>
#include <optional>
#include <unordered_set>
#include <thread>

#include <Utilities.hpp>
#include <Texture.hpp>
