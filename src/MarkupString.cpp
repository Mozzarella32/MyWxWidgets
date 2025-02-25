#include "MarkupString.hpp"

wxString MarkupString::Escape(const wxString& str) {
	wxString result;
	for (size_t i = 0; i < str.length(); ++i) {
		switch ((wchar_t)str[i]) {
		case L'&':
			result += "&amp;";
			break;
		case L'\'':
			result += "&apos;";
			break;
		case L'\"':
			result += "&quot;";
			break;
		case L'<':
			result += "&lt;";
			break;
		case L'>':
			result += "&gt;";
			break;
		default:
			result += str[i];
			break;
		}
	}
	return result;
}

MarkupString::MarkupString(wxWindow* window) {
	InheretedColours = true;
	BeginForegroundColour(window->GetForegroundColour());
	BeginBackgroundColour(window->GetBackgroundColour());
}

MarkupString& MarkupString::EndInharitance() {
	EndAttribute();
	EndAttribute();
	return *this;
}

MarkupString& MarkupString::BeginAttribute(const wxString& attrib) {
	*this += "<span " + attrib + ">";
	return *this;
}

MarkupString& MarkupString::EndAttribute() {
	*this += "</span>";
	return *this;
}

MarkupString& MarkupString::BeginForegroundColour(const wxColour& colour) {
	*this += "<span foreground=\'rgb(" +
		std::to_string(colour.Red()) + "," +
		std::to_string(colour.Green()) + "," +
		std::to_string(colour.Blue()) + ")\'>";
	return *this;
}

MarkupString& MarkupString::BeginBackgroundColour(const wxColour& colour) {
	*this += "<span background=\'rgb(" +
		std::to_string(colour.Red()) + "," +
		std::to_string(colour.Green()) + "," +
		std::to_string(colour.Blue()) + ")\'>";
	return *this;
}

MarkupString& MarkupString::BeginBold() {
	*this += "<b>";
	return *this;
}

MarkupString& MarkupString::EndBold() {
	*this += "</b>";
	return *this;
}

MarkupString& MarkupString::BeginBig() {
	*this += "<big>";
	return *this;
}

MarkupString& MarkupString::EndBig() {
	*this += "</big>";
	return *this;
}

MarkupString& MarkupString::BeginItalic() {
	*this += "<i>";
	return *this;
}

MarkupString& MarkupString::EndItalic() {
	*this += "</i>";
	return *this;
}

MarkupString& MarkupString::BeginSmall() {
	*this += "<small>";
	return *this;
}

MarkupString& MarkupString::EndSmall() {
	*this + "</small>";
	return *this;
}

MarkupString& MarkupString::BeginUnderlined() {
	*this += "<u>";
	return *this;
}

MarkupString& MarkupString::EndUnderlined() {
	*this += "</u>";
	return *this;
}
