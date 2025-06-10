#pragma once

#include "pch.hpp"
#include "wx/glcanvas.h"

class AppWithGlContext : public wxApp {
  std::optional<wxGLContext> Context;
  std::unordered_set<wxGLCanvas *> Canvases;
  bool isInitilized = false;
  wxGLContextAttrs RealContextAttrs;

public:
  void AppWithGLContext();
  
  virtual wxGLAttributes GetGLAttrs() const;

  virtual wxGLContextAttrs GetGLContextAttrs() const;

  virtual void OnGLInit() {};

  const wxGLContextAttrs &GetRealGLContextAttrs() const;

  bool Initilized(wxGLCanvas *Canvas);

  bool BindGLContext();

  const std::optional<wxGLContext> &GetContext() const;

  void RegisterGLCanvas(wxGLCanvas *Canvas);
  void UnRegisterGLCanvas(wxGLCanvas *Canvas);

};

class wxGLCanvasWithAppContext : public wxGLCanvas {
public:
  AppWithGlContext *App;
  std::optional<std::function<void(wxSizeEvent&)>> OnSize; 

public:
  wxGLCanvasWithAppContext(AppWithGlContext *App, wxWindow *parent,
                           wxWindowID id = wxID_ANY,
                           const wxPoint &pos = wxDefaultPosition,
                           const wxSize &size = wxDefaultSize, long style = 0,
                           const wxString &name = wxGLCanvasName,
                           const wxPalette &palette = wxNullPalette);

  virtual ~wxGLCanvasWithAppContext();

  bool BindContext();

  void SetOnSize(std::function<void(wxSizeEvent&)> Func);
};

// class GLInitiliser : public wxGLCanvas {
//   wxGLContext *Context;
//   std::function<void(void)> InitFunction;
//   std::function<void(void)> GlLoadFunction;

// public:
//   GLInitiliser(wxWindow *parent, wxGLAttributes &glAttrs,
//                wxGLContextAttrs &cxtAttrs,
//                std::function<void(void)> GlLoadFunction,
//                std::function<void(void)> InitFunction)
//       : wxGLCanvas(parent, glAttrs), InitFunction(InitFunction),
//         GlLoadFunction(GlLoadFunction) {
//     static bool Init = false;

// Context = new wxGLContext(this, nullptr, &cxtAttrs);

//     if (!Context->IsOK()) {
//       std::cout << "Opengl Context not OK!\n";
//       exit(1);
//     }

//     // SetCurrent(*Context);

//     wxASSERT_MSG(parent->GetChildren().size() == 1 && !Init,
//                  "This class sould be the first wiged to be created in your "
//                  "frame and only be instanciated once");

//     Bind(wxEVT_SIZE, [this](wxSizeEvent &evt) { this->OnSize(evt); });

//     CallAfter([this]() { SetSize(wxSize(1, 1)); });

//     Show();
//   }

//   wxGLContext *RetriveContextAndClose() {
//     SetSize(wxSize(0, 0));
//     Hide();
//     Disable();
//     Close();
//     return Context;
//   }

//   void OnSize(wxSizeEvent &event) {

//     if (!IsShownOnScreen())
//       return;

//     static bool init = false;
//     if (init)
//       return;
//     init = true;

//     SetCurrent(*Context);
//     GlLoadFunction();
//     Unbind(wxEVT_SIZE, &GLInitiliser::OnSize, this);
//     CallAfter([this]() { InitFunction(); });
//   }
// };

// class GLFrameIndependentInitiliser : public wxFrame {
//   GLInitiliser *glinit;

// public:
//   // The Init Function Will be called after the GlLoadFunction was called
//   GLFrameIndependentInitiliser(wxGLAttributes &glAttrs,
//                                wxGLContextAttrs &cxtAttrs,
//                                std::function<void(void)> GlLoadFunction,
//                                std::function<void(void)> InitFunktion)
//       : wxFrame(NULL, wxID_ANY, "GLInit") {

//     if (!wxGLCanvas::IsDisplaySupported(glAttrs)) {
//       std::cout << "IsDisplaySupported returned false";
//       exit(1);
//     }

//     glinit =
//         new GLInitiliser(this, glAttrs, cxtAttrs, GlLoadFunction,
//         InitFunktion);
//     Show();
//   }

//   wxGLContext *RetriveContextAndClose() {
//     wxGLContext *Context = glinit->RetriveContextAndClose();
//     SetSize(wxSize(0, 0));
//     Hide();
//     Disable();
//     Close();
//     return Context;
//   }
// };
