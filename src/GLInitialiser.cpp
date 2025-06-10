#include "GLInitialiser.hpp"

AppWithGlContext::AppWithGlContext(){
    if (!wxGLCanvas::IsDisplaySupported(GetGLAttrs())) {
    wxMessageBox("Default GlAttrebutes not suppoerted", "Error", wxICON_ERROR);
    exit(1);
  }
}

wxGLAttributes AppWithGlContext::GetGLAttrs() const {
  wxGLAttributes glAttrs;
  glAttrs.PlatformDefaults().Defaults().EndList();
  return glAttrs;
}

wxGLContextAttrs AppWithGlContext::GetGLContextAttrs() const {
  wxGLContextAttrs glCtxAttrs;
  glCtxAttrs.CoreProfile().OGLVersion(4, 5).EndList();
  return glCtxAttrs;
}

const wxGLContextAttrs &AppWithGlContext::GetRealGLContextAttrs() const {
  return RealContextAttrs;
}

bool AppWithGlContext::Initilized(wxGLCanvas *Canvas) {
  if (isInitilized)
    return true;

  if (!Canvas->IsShownOnScreen())
    return false;

  if (!Context) {
    wxMessageBox("Initilized called with canvas but no Context jet", "Error",
                 wxICON_ERROR);
    return false;
  }

  Canvas->SetCurrent(Context.value());

  if (!gladLoadGL()) {
    wxMessageBox("Failed to initialize GLAD!", "Error", wxICON_ERROR);
    exit(1);
  }

  isInitilized = true;
  OnGLInit();
  return true;
}

bool AppWithGlContext::BindGLContext() {
  if (!Context) {
    return false;
  }
  if (Canvases.empty()) {
    return false;
  }
  (*Canvases.begin())->SetCurrent(Context.value());
  return true;
}

const std::optional<wxGLContext> &AppWithGlContext::GetContext() const {
  return Context;
}

void AppWithGlContext::RegisterGLCanvas(wxGLCanvas *Canvas) {
  if (!Context) {
    RealContextAttrs = GetGLContextAttrs();
    Context = wxGLContext(Canvas, nullptr, &RealContextAttrs);

    if (!Context->IsOK()) {
      wxMessageBox("Failed to create opengl context", "Error", wxICON_ERROR);
      exit(1);
    }

    Canvas->SetCurrent(Context.value());
  }
  Canvases.insert(Canvas);
  Canvas->Show();
}

void AppWithGlContext::UnRegisterGLCanvas(wxGLCanvas *Canvas) {
  Canvases.erase(Canvas);
}

wxGLCanvasWithAppContext::wxGLCanvasWithAppContext(
    AppWithGlContext *App, wxWindow *parent, wxWindowID id, const wxPoint &pos,
    const wxSize &size, long style, const wxString &name,
    const wxPalette &palette)
    : wxGLCanvas(parent, App->GetGLAttrs(), id, pos, size, style, name,
                 palette), App(App) {

  App->RegisterGLCanvas(this);

  Bind(wxEVT_SIZE, [this](wxSizeEvent &evt) {
    if(!this->App->Initilized(this)){
      return;
    }
    if (OnSize) {
      OnSize.value()(evt);
    }
  });
}
wxGLCanvasWithAppContext::~wxGLCanvasWithAppContext() {
  App->UnRegisterGLCanvas(this);
}

bool wxGLCanvasWithAppContext::BindContext() {
  if (!App->Initilized(this)) {
    return false;
  }
  if (!App->GetContext())
    return false;
  SetCurrent(App->GetContext().value());
  return true;
}

void wxGLCanvasWithAppContext::SetOnSize(
    std::function<void(wxSizeEvent &)> Func) {
  OnSize = Func;
}
