#pragma once

#include "pch.hpp"

class GLContextBinder :public wxFrame {
	wxGLCanvas* Canvas;
public:
	GLContextBinder();

	void BindContext(wxGLContext* Context);
};
