#pragma once

#include "pch.h"

class GLEWInitiliser : public wxGLCanvas {
	wxGLContext* Context;
public:
	GLEWInitiliser(wxWindow* parent, std::function<void(void)> InitFunktion = []() {}) : wxGLCanvas(parent) {
		static bool Init = false;

		Context = new wxGLContext(this);
		SetCurrent(*Context);

		wxASSERT_MSG(parent->GetChildren().size() == 1 && !Init, "This class sould be the first wiged to be created in your frame and only be instanciated once");

		GLenum glewInitResult = glewInit();
		if (glewInitResult != GLEW_OK) {
			const GLubyte* errorString = glewGetErrorString(glewInitResult);
			wxASSERT_MSG(false, wxString::Format("GLEW-Fehler: %s", errorString));
		}

		InitFunktion();
	}

	wxGLContext* RetriveContextAndClose() {
		SetSize(wxSize(0, 0));
		Hide();
		Disable();
		Close();
		return Context;
	}
};


class GLEWFrameIndependentInitiliser :public wxFrame {
	wxGLContext* Context;
public:
	GLEWFrameIndependentInitiliser(std::function<void(void)> InitFunktion = []() {}) :wxFrame(NULL, wxID_ANY, "GlewInit") {
		GLEWInitiliser* glewinit = new GLEWInitiliser(this,InitFunktion);
		Context = glewinit->RetriveContextAndClose();
	}

	wxGLContext* RetriveContextAndClose() {
		SetSize(wxSize(0, 0));
		Hide();
		Disable();
		Close();
		return Context;
	}
};
