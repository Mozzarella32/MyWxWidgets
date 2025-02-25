#pragma once

#include "pch.hpp"

class RectangelMarker : public wxWindow {
private:
	wxBrush Brush = wxBrush(*wxRED_BRUSH);

	wxBitmap backgroundBitmap;

	void OnSize(wxSizeEvent& evt);

	void OnPaint(wxPaintEvent& evt);

public:
	int BorderSize = 20;

	RectangelMarker(wxWindow* parent, wxWindowID id = wxID_ANY);

	void WrapAround(wxWindow* window);
};
