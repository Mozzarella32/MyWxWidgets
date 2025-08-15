#include "RectangleMarker.hpp"

void RectangelMarker::OnSize(wxSizeEvent& evt) {
	backgroundBitmap.Create(GetSize());
	backgroundBitmap.UseAlpha();

	wxMemoryDC dc(backgroundBitmap);

	dc.SetBrush(*wxTRANSPARENT_BRUSH);
	dc.Clear();

	wxGraphicsContext* gc = wxGraphicsContext::Create(dc);

	if (!gc)return;

	gc->SetBrush(*wxRED_BRUSH);

	wxAffineMatrix2D ToFull{};
	ToFull.Scale(GetSize().x, GetSize().y);

	// auto Top = ToFull.TransformPoint({ 0,0 });
	// auto Bottom = ToFull.TransformPoint({ 1,0.2 });

	wxPoint Stripes{ GetSize().x / BorderSize,GetSize().y / BorderSize };
	wxPoint Scales = Stripes * 2;

	auto RectSize = ToFull.TransformPoint({ 1 / double(Scales.x - 1),1 / double(Scales.y - 1) });
	for (wxDouble x = 0; x < Scales.x; x++) {
		for (wxDouble y = 0; y < Scales.y; y++) {
			if ((x == 0 && y == 0) ||
				(x == 0 && y == Scales.y - 2) ||
				(x == Scales.x - 2 && y == 0) ||
				(x == Scales.x - 2 && y == Scales.y - 2) ||
				(x == 0 && int(y) % 2 == 1) ||
				(int(x) % 2 == 1 && y == 0) ||
				(x == Scales.x - 2 && int(y) % 2 == 1) ||
				(int(x) % 2 == 1 && y == Scales.y - 2)) {
				auto Pos = ToFull.TransformPoint({ x / wxDouble(Scales.x - 1),y / wxDouble(Scales.y - 1) });
				gc->DrawRectangle(Pos.m_x, Pos.m_y, RectSize.m_x, RectSize.m_y);
			}
		}
	}

	evt.Skip();
}

void RectangelMarker::OnPaint(wxPaintEvent& evt) {
	wxPaintDC dc(this);
	if (backgroundBitmap.IsOk()) {
		dc.DrawBitmap(backgroundBitmap, 0, 0);
	}

	evt.Skip();
}

RectangelMarker::RectangelMarker(wxWindow* parent, wxWindowID id)
	: wxWindow(parent, id, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE) {
	SetBackgroundStyle(wxBG_STYLE_PAINT);
	SetBackgroundColour(wxColour(0, 0, 0, 0));
	Bind(wxEVT_PAINT, &RectangelMarker::OnPaint, this);
	Bind(wxEVT_SIZE, &RectangelMarker::OnSize, this);
}

void RectangelMarker::WrapAround(wxWindow* window) {
	Show(true);
	SetPosition(window->GetPosition() - wxPoint(BorderSize, BorderSize));
	SetSize(window->GetSize() + 2 * wxSize(BorderSize, BorderSize));
}
