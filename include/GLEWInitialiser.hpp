#pragma once

#include "pch.hpp"

class GLEWInitiliser : public wxGLCanvas {
	wxGLContext* Context;
	std::function<void(void)> InitFunktion;
public:
	GLEWInitiliser(wxWindow* parent, wxGLContextAttrs& cxtAttrs, std::function<void(void)> InitFunktion = []() {}) : wxGLCanvas(parent), InitFunktion(InitFunktion) {
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
	GLEWInitiliser* glewinit;
public:
	//The Init Function Will be called after the GLEW Initilisation
	GLEWFrameIndependentInitiliser(wxGLContextAttrs& cxtAttrs, std::function<void(void)> InitFunktion = []() {}) :wxFrame(NULL, wxID_ANY, "GlewInit") {
		glewinit = new GLEWInitiliser(this, cxtAttrs, InitFunktion);
		Show();
	}

	wxGLContext* RetriveContextAndClose() {
		wxGLContext* Context = glewinit->RetriveContextAndClose();
		SetSize(wxSize(0, 0));
		Hide();
		Disable();
		Close();
		return Context;
	}
};

