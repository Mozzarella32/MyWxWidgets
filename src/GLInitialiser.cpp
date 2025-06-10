#include "GLInitialiser.hpp"

FrameWithGlContext::FrameWithGlContext() {
  if (!wxGLCanvas::IsDisplaySupported(GetGLAttrs())) {
    wxMessageBox("Default GlAttrebutes not suppoerted", "Error", wxICON_ERROR);
    exit(1);
  }
}

wxGLAttributes FrameWithGlContext::GetGLAttrs() const {
  wxGLAttributes glAttrs;
  glAttrs.PlatformDefaults().Defaults().EndList();
  return glAttrs;
}

wxGLContextAttrs FrameWithGlContext::GetGLContextAttrs() const {
  wxGLContextAttrs glCtxAttrs;
  glCtxAttrs.CoreProfile().OGLVersion(4, 5).EndList();
  return glCtxAttrs;
}

const wxGLContextAttrs &FrameWithGlContext::GetRealGLContextAttrs() const {
  return RealContextAttrs;
}

bool FrameWithGlContext::Initilized(wxGLCanvas *Canvas) {
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

bool FrameWithGlContext::BindGLContext() {
  if (!Context) {
    return false;
  }
  if (Canvases.empty()) {
    return false;
  }
  (*Canvases.begin())->SetCurrent(Context.value());
  return true;
}

const std::optional<wxGLContext> &FrameWithGlContext::GetContext() const {
  return Context;
}

void FrameWithGlContext::RegisterGLCanvas(wxGLCanvas *Canvas) {
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
  Canvas->CallAfter([Canvas]() { Canvas->SetSize(wxSize(1, 1)); });
  Canvas->Show();
}

void FrameWithGlContext::UnRegisterGLCanvas(wxGLCanvas *Canvas) {
  Canvases.erase(Canvas);
}

wxGLCanvasWithFrameContext::wxGLCanvasWithFrameContext(
    FrameWithGlContext *Frame, wxWindow *parent, wxWindowID id,
    const wxPoint &pos, const wxSize &size, long style, const wxString &name,
    const wxPalette &palette)
    : wxGLCanvas(parent, Frame->GetGLAttrs(), id, pos, size, style, name,
                 palette),
      Frame(Frame) {

  Frame->RegisterGLCanvas(this);

  Bind(wxEVT_SIZE, [this](wxSizeEvent &evt) {
    if (!this->Frame->Initilized(this)) {
      return;
    }
    if (OnSize) {
      OnSize.value()(evt);
    }
  });
}
wxGLCanvasWithFrameContext::~wxGLCanvasWithFrameContext() {
  Frame->UnRegisterGLCanvas(this);
}

bool wxGLCanvasWithFrameContext::BindContext() {
  if (!Frame->Initilized(this)) {
    return false;
  }
  if (!Frame->GetContext())
    return false;
  SetCurrent(Frame->GetContext().value());
  return true;
}

void wxGLCanvasWithFrameContext::SetOnSize(
    std::function<void(wxSizeEvent &)> Func) {
  OnSize = Func;
}
