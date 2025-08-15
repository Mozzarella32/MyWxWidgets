#include "ElementLayerer.hpp"
#include "../submodules/Instrumentor/Instrumentor.hpp"

#include "Utility.hpp"

ElementLayerer::ElementLayerer(wxWindow* RootParent, wxSizer* RootSizer, StyleFunktion SFunk)
	:RootParent(RootParent), RootSizer(RootSizer), SFunk(SFunk) {
	PROFILE_FUNKTION;
	ContentLayer.push(RootParent);
	ContentLayerSizer.push(RootSizer);
}

void ElementLayerer::SetSFunk(StyleFunktion SFunk){
	this->SFunk = SFunk;
}

wxSizer* ElementLayerer::GetRootSizer() const {
	PROFILE_FUNKTION;
	return RootSizer;
}

wxWindow* ElementLayerer::GetRootParent() const {
	PROFILE_FUNKTION;
	return RootParent;
}

wxSizer* ElementLayerer::Sizer() const {
	PROFILE_FUNKTION;
	return ContentLayerSizer.top();
}

wxWindow* ElementLayerer::Parent() const {
	PROFILE_FUNKTION;
	return ContentLayer.top();
}

void ElementLayerer::PostAddContent() {
	PROFILE_FUNKTION;
	PROFILE_SCOPE_ID_START("BLACK and WHITE", 0);

	wxWindow* Parent = RootParent->GetParent();
	if (Parent == nullptr)Parent = RootParent;
	ForAllChildren(RootParent,SFunk);
	
	PROFILE_SCOPE_ID_END(0);

	wxCommandEvent* evtscroll = new wxCommandEvent(wxEVT_FIT_REQUEST, ContentLayer.top()->GetId());
	evtscroll->SetEventObject(ContentLayer.top());
	ContentLayer.top()->GetEventHandler()->QueueEvent(evtscroll);
}

wxCollapsiblePane* ElementLayerer::BeginCollapsible(const MarkupString& string, int leftOffset) {
	PROFILE_SCOPE(("BeingCollapsible" + string.ToStdString()).c_str());
	PROFILE_SCOPE_ID_START("Create Collapsible", 0);
	auto* collab = new TransmitHideShowFromCollapsible(ContentLayer.top(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxCP_NO_TLW_RESIZE);
	PROFILE_SCOPE_ID_END(0);
	PROFILE_SCOPE_ID_START("SetMarkupLable", 1);
	collab->SetLabelMarkup(string);
	PROFILE_SCOPE_ID_END(1);
	//collab->Refresh();

	PROFILE_SCOPE_ID_START("Collapse(false)", 2);
	collab->Collapse(false);
	PROFILE_SCOPE_ID_END(2);

	ContentLayerSizer.top()->Add(collab, 0, wxALL, 5);

	auto* pane = collab->GetPane();

	ContentLayer.push(pane);

	PROFILE_SCOPE_ID_START("CreateInside", 3);
	auto* paneSizer = new wxBoxSizer(wxVERTICAL);
	if (leftOffset == 0) {
		ContentLayerSizer.push(paneSizer);
		pane->SetSizerAndFit(paneSizer);
	}
	else {
		paneSizer->AddSpacer(leftOffset);

		auto* paneRightSizer = new wxBoxSizer(wxVERTICAL);
		paneSizer->Add(paneRightSizer, 0);
		ContentLayerSizer.push(paneRightSizer);
		pane->SetSizerAndFit(paneSizer);
	}
	PROFILE_SCOPE_ID_END(3);

	PROFILE_SCOPE_ID_START("PostAddContent", 4);
	PostAddContent();
	PROFILE_SCOPE_ID_END(4);

	collab->Bind(wxEVT_FIT_REQUEST, [collab](wxCommandEvent& evt) {
		PROFILE_SCOPE("FitRequest On Collap");
		evt.Skip();
		bool now = collab->IsCollapsed();
		collab->Collapse(!now);
		collab->Collapse(now);
		
		wxCommandEvent* evtlayout = new wxCommandEvent(wxEVT_LAYOUT_REQUEST);
		evtlayout->SetEventObject(collab);
		collab->GetEventHandler()->QueueEvent(evtlayout);

		});

	collab->Bind(wxEVT_COLLAPSIBLEPANE_CHANGED, [this, collab](wxCollapsiblePaneEvent& evt) {
		PROFILE_SCOPE("Collapsible Changed");
		evt.Skip();
		collab->Layout();
		collab->Refresh();
		collab->Update();
		wxCommandEvent* evtfit = new wxCommandEvent(wxEVT_FIT_REQUEST);
		evtfit->SetEventObject(collab);
		collab->GetEventHandler()->QueueEvent(evtfit);
		
		wxCommandEvent* evtlayout = new wxCommandEvent(wxEVT_LAYOUT_REQUEST);
		evtlayout->SetEventObject(collab);
		collab->GetEventHandler()->QueueEvent(evtlayout);

		//PostAddContent();
		});
	return collab;
}

void ElementLayerer::EndCollapsible() {
	PROFILE_FUNKTION;
	ContentLayer.pop();
	ContentLayerSizer.pop();
}

wxStaticBox* ElementLayerer::BeginBox(const MarkupString& string){
	PROFILE_SCOPE(("BeingBox" + string.ToStdString()).c_str());

	PROFILE_SCOPE_ID_START("Create Box", 0);
	auto* box = new wxStaticBox(ContentLayer.top(), wxID_ANY,wxEmptyString);
	PROFILE_SCOPE_ID_END(0);
	PROFILE_SCOPE_ID_START("SetMarkupLable", 1);
	box->SetLabelMarkup(string);
	PROFILE_SCOPE_ID_END(1);

	auto StaticBoxSizer = new wxStaticBoxSizer(box,wxVERTICAL);

	ContentLayerSizer.top()->Add(StaticBoxSizer, 0, wxALL, 5);
	//ContentLayer.push(box);
	ContentLayerSizer.push(StaticBoxSizer);

	PROFILE_SCOPE_ID_START("PostAddContent", 4);
	PostAddContent();
	PROFILE_SCOPE_ID_END(4);
	return box;
}

void ElementLayerer::EndBox(){
	PROFILE_FUNKTION;
	//ContentLayer.pop();
	ContentLayerSizer.pop();
}

void ElementLayerer::Reset() {
	PROFILE_FUNKTION;

	while (ContentLayerSizer.size() > 1)ContentLayerSizer.pop();
	while (ContentLayer.size() > 1)ContentLayer.pop();

	ContentLayerSizer.top()->Clear(true);
}

void ElementLayerer::CollapsAll(bool Collaps){
	ForAllChildren(RootParent, [Collaps](wxWindow* w){
		if (auto coll = dynamic_cast<wxCollapsiblePane*>(w); coll != nullptr) {
			coll->Collapse(Collaps);
	}
		});
}
