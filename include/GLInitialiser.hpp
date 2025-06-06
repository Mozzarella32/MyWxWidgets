#pragma once

#include "pch.hpp"

class GLInitiliser : public wxGLCanvas {
	wxGLContext* Context;
	std::function<void(void)> InitFunction;
	std::function<void(void)> GlLoadFunction;
public:
	GLInitiliser(wxWindow* parent, wxGLContextAttrs& cxtAttrs, std::function<void(void)> GlLoadFunction, std::function<void(void)> InitFunction) : wxGLCanvas(parent), InitFunction(InitFunction), GlLoadFunction(GlLoadFunction) {
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
		static bool init = false;
		if(init) return;
		init = true;

		SetCurrent(*Context);
		GlLoadFunction();
		Unbind(wxEVT_SIZE, &GLInitiliser::OnSize, this);
		CallAfter([this]() {
			InitFunction();
			});
	}
};


class GLFrameIndependentInitiliser :public wxFrame {
	GLInitiliser* glinit;
public:
	//The Init Function Will be called after the GlLoadFunction was called
	GLFrameIndependentInitiliser(wxGLContextAttrs& cxtAttrs, std::function<void(void)> GlLoadFunction, std::function<void(void)> InitFunktion):wxFrame(NULL, wxID_ANY, "GLInit") {
		glinit = new GLInitiliser(this, cxtAttrs, GlLoadFunction, InitFunktion);
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

