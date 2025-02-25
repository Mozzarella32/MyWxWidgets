//#pragma once
//
//#include <wx/wx.h>
//
//class ScalingBitmap : public wxPanel {
//
//	/*void OnPaint(wxPaintEvent& evt) {
//		if (!Image.IsOk())return;
//		wxPaintDC dc(m_StaticBitmap);
//		int newWidth, newHeight;
//		auto Size = GetClientSize();
//		newWidth = Size.GetWidth();
//		newHeight = Size.GetHeight();
//
//		dc.Clear();
//
//		if (newWidth != Bitmap.GetWidth() || newHeight != Bitmap.GetHeight())
//		{
//			if (Image.GetWidth() > Image.GetHeight())
//			{
//				newHeight = newWidth * Image.GetHeight() / Image.GetWidth();
//			}
//			else
//			{
//				newWidth = newHeight * Image.GetWidth() / Image.GetHeight();
//			}
//			if (newWidth == 0 || newHeight == 0) {
//				Bitmap = wxNullBitmap;
//			}
//			else {
//				Bitmap = wxBitmap(Image.Scale(newWidth, newHeight));
//			}
//		}
//		auto BitmapSize = Bitmap.GetSize();
//		int x = (newWidth - BitmapSize.GetWidth()) / 2;
//		int y = (newHeight - BitmapSize.GetHeight()) / 2;
//
//		if (Bitmap.IsOk()) {
//			dc.DrawBitmap(Bitmap, x, y, false);
//		}
//	}*/
//
//	wxBitmap Bitmap;
//
//public:
//	ScalingBitmap(wxWindow* Parent, wxWindowID id = wxID_ANY) : wxPanel(Parent, id, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE) {
//		auto* sizer = new wxBoxSizer(wxVERTICAL);
//		m_StaticBitmap = new wxStaticBitmap(this, wxID_ANY, wxNullIcon,wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE);
//		/*m_StaticBitmap->SetBackgroundStyle(wxBG_STYLE_PAINT);
//		m_StaticBitmap->Bind(wxEVT_PAINT, &ScalingBitmap::OnPaint, this);
//	*/	sizer->Add(m_StaticBitmap,1,wxEXPAND|wxALL,0);
//		SetSizerAndFit(sizer);
//	}
//	
//	wxStaticBitmap* m_StaticBitmap;
//
//	//wxImage Image;
//};
