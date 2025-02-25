#include "Utility.hpp"

wxDEFINE_EVENT(wxEVT_FIT_REQUEST, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_LAYOUT_REQUEST, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_ADJUST_REQUEST, wxCommandEvent);

void ForAllChildren(wxWindow* w, FunktionForAllChildren f) {
	if (w == nullptr)return;
	f(w);

	const wxWindowList& children = w->GetChildren();
	for (auto node = children.GetFirst(); node; node = node->GetNext())
		ForAllChildren(dynamic_cast<wxWindow*>(node->GetData()), f);
}

TempDirManager::TempDirManager() {
	std::filesystem::remove_all(TempDir);
	std::filesystem::create_directories(TempDir);
}

TempDirManager::~TempDirManager() {
	std::filesystem::remove_all(TempDir);
}

TransmitHideShowFromCollapsible::TransmitHideShowFromCollapsible(wxWindow* Parent, wxWindowID id, const wxString& lable, const wxPoint& pos, const wxSize& size, long style, const wxValidator& val, const wxString& name)
	:wxCollapsiblePane(Parent, id, lable, pos, size, style, val, name)
{
	Bind(wxEVT_COLLAPSIBLEPANE_CHANGED, [this](wxCollapsiblePaneEvent& evt) {
		ForAllChildren(this, [evt](wxWindow* w) {
			if (auto Reciver = dynamic_cast<ReciveHideShowFromCollapsible*>(w); Reciver != nullptr) {
				Reciver->OnCollapsible();
			}
			});

		evt.Skip();
		});
}
