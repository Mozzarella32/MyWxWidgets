#pragma once

#include "pch.hpp"

class MarkupString : public wxString {
private:
	wxString Escape(const wxString& str);

	bool InheretedColours = false;
public:
	explicit MarkupString(const wxString& str) : wxString(str) {}

	MarkupString() {};
	MarkupString(wxWindow* window);

	MarkupString& EndInharitance();

	template<class T>
	MarkupString& operator<<(const T& t) {
		std::wstringstream ss;
		ss << t;
		*this += Escape(ss.str());
		return *this;
	}

	MarkupString& BeginAttribute(const wxString& attrib);

	MarkupString& EndAttribute();

	MarkupString& BeginForegroundColour(const wxColour& colour);

	MarkupString& BeginBackgroundColour(const wxColour& colour);

	MarkupString& BeginBold();

	MarkupString& EndBold();

	MarkupString& BeginBig();

	MarkupString& EndBig();

	MarkupString& BeginItalic();

	MarkupString& EndItalic();

	MarkupString& BeginSmall();

	MarkupString& EndSmall();

	MarkupString& BeginUnderlined();

	MarkupString& EndUnderlined();
};

