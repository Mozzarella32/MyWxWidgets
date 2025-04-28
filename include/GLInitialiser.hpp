#pragma once

#include "pch.hpp"

class GLInitiliser : public wxGLCanvas {
	wxGLContext* Context;
	std::function<void(void)> InitFunction;
	std::function<void(void)> GlLoadFunction;
public:
	GLInitiliser(wxWindow* parent, wxGLContextAttrs& cxtAttrs, std::function<void(void> GlLoadFunction, std::function<void(void)> InitFunction = []() {}) : wxGLCanvas(parent), InitFunction(InitFunction), GlLoadFunction(GlLoadFunction) {
		static bool Init = false;

		Context = new wxGLContext(this,nullptr,&cxtAttrs);
		SetCurrent(*Context);

		wxASSERT_MSG(parent->GetChildren().size() == 1 && !Init, "This class sould be the first wiged to be created in your frame and only be instanciated once");

		Bind(wxEVT_SIZE, [this](wxSizeEvent& evt){this->OnSize(evt);});


		CallAfter([this](){
			SetSize(wxSize(1,1));
		});

		Show();
	}

	wxGLContext* RetriveContextAndClose() {
		SetSize(wxSize(0, 0));
		Hide();
		Disable();
		Close();
		return Context;
	}

	void OnSize(wxSizeEvent& event) {
		SetCurrent(*Context);
		GlLoadFunction();
		// GLenum glewInitResult = glewInit();
		// if (glewInitResult != GLEW_OK) {
			// const GLubyte* errorString = glewGetErrorString(glewInitResult);
			// std::cout << "GLEW-Fehler: " << errorString << std::endl;
			// wxASSERT_MSG(false, wxString::Format("GLEW-Fehler: %s", errorString));
		// }
		Unbind(wxEVT_SIZE, &GLEWInitiliser::OnSize, this);
		CallAfter([=]() {
			InitFunction();
			});
	}
};


class GLFrameIndependentInitiliser :public wxFrame {
	GLInitiliser* glinit;
public:
	//The Init Function Will be called after the GLEW Initilisation
	GLEWFrameIndependentInitiliser(wxGLContextAttrs& cxtAttrs, std::function<void(void)> InitFunktion = []() {}) :wxFrame(NULL, wxID_ANY, "GlewInit") {
		glinit = new GLInitiliser(this, cxtAttrs, InitFunktion);
		Show();
	}

	wxGLContext* RetriveContextAndClose() {
		wxGLContext* Context = glinit->RetriveContextAndClose();
		SetSize(wxSize(0, 0));
		Hide();
		Disable();
		Close();
		return Context;
	}
};

