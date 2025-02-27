#pragma once

#include "pch.hpp"

class GLEWInitiliser : public wxGLCanvas {
	wxGLContext* Context;
	std::function<void(void)> InitFunktion;
public:
	GLEWInitiliser(wxWindow* parent, std::function<void(void)> InitFunktion = []() {}) : wxGLCanvas(parent), InitFunktion(InitFunktion) {
		static bool Init = false;

		Context = new wxGLContext(this);
		SetCurrent(*Context);

		wxASSERT_MSG(parent->GetChildren().size() == 1 && !Init, "This class sould be the first wiged to be created in your frame and only be instanciated once");

		Bind(wxEVT_SIZE, &GLEWInitiliser::OnSize, this);
	}

	wxGLContext* RetriveContextAndClose() {
		SetSize(wxSize(0, 0));
		Hide();
		Disable();
		Close();
		return Context;
	}

	void OnSize(wxSizeEvent& event) {
		GLenum glewInitResult = glewInit();
		if (glewInitResult != GLEW_OK) {
			const GLubyte* errorString = glewGetErrorString(glewInitResult);
			std::cout << "GLEW-Fehler: " << errorString << std::endl;
			wxASSERT_MSG(false, wxString::Format("GLEW-Fehler: %s", errorString));
		}
		Unbind(wxEVT_SIZE, &GLEWInitiliser::OnSize, this);
		CallAfter([=]() {
			InitFunktion();
			});
	}
};


class GLEWFrameIndependentInitiliser :public wxFrame {
	wxGLContext* Context;
public:
	//The Init Function Will be called after the GLEW Initilisation
	GLEWFrameIndependentInitiliser(std::function<void(void)> InitFunktion = []() {}) :wxFrame(NULL, wxID_ANY, "GlewInit") {
		GLEWInitiliser* glewinit = new GLEWInitiliser(this, InitFunktion);
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
