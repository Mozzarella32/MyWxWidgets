#pragma once

#include "pch.hpp"

#include "Instrumentor/Instrumentor.h"

class ElementLayerer {
public:
	typedef std::function<void(wxWindow*)>  StyleFunktion;

private:
	wxWindow* RootParent;
	wxSizer* RootSizer;

	std::stack<wxWindow*> ContentLayer;
	std::stack<wxSizer*> ContentLayerSizer;

	StyleFunktion SFunk;

public:
	
	inline static const StyleFunktion DefaultSFunk = [](wxWindow* w) {
		PROFILE_SCOPE("BLACKWHITE");
		w->SetBackgroundColour(wxColour("Black"));
		w->SetForegroundColour(wxColour("White"));
		};

	ElementLayerer(wxWindow* RootParent, wxSizer* RootSizer, StyleFunktion SFunk = DefaultSFunk);

	void SetSFunk(StyleFunktion SFunk);

	wxSizer* GetRootSizer()const;
	wxWindow* GetRootParent() const;

	wxSizer* Sizer()const;
	wxWindow* Parent() const;

	void PostAddContent();

	wxCollapsiblePane* BeginCollapsible(const MarkupString& string, int leftOffset = 0);
	void EndCollapsible();

	wxStaticBox* BeginBox(const MarkupString& string);
	void EndBox();

	//BeginBox(constMarkupString&string)

	void Reset();

	void CollapsAll(bool Collaps);
};