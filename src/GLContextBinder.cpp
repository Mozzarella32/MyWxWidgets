#include "GLContextBinder.hpp"

GLContextBinder::GLContextBinder() :wxFrame(NULL, wxID_ANY, "ContextBinder") {
	Disable();
	Hide();
	SetSize(wxSize(0, 0));
	Canvas = new wxGLCanvas(this);
	Canvas->Disable();
	//Canvas->Hide();
	Canvas->SetSize(wxSize(0, 0));
}

void GLContextBinder::BindContext(wxGLContext* Context) {
	Canvas->SetCurrent(*Context);
	ERRORLOG("GLContextBinder: BoundContext");
	CONTEXTCHECK;
}
