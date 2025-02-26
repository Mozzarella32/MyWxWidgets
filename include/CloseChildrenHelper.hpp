#pragma once

#include "pch.hpp"
struct CloseTypeType {};

template<typename T>
concept IsCloseType = std::derived_from<T, CloseTypeType>;

#define DECLARE_CLOSE_TYPE(name)struct CloseType_##name : public CloseTypeType{};

template<IsCloseType CloseType>
class CloseableChildHelper {
protected:
public:
	virtual bool TryCloseVeto() = 0;
};

template<IsCloseType... CloseType>
	requires (sizeof...(CloseType) > 0)
class CloseChildrenWindow{
private:
	template<IsCloseType CheckCloseType>
	bool CheckItem(wxWindowList::iterator::value_type item) {
		CloseableChildHelper<CheckCloseType>* closable = dynamic_cast<CloseableChildHelper<CheckCloseType>*>(item);
		return closable && closable->TryCloseVeto();
	}
protected:
	bool TryCloseChildrenVeto() {
		std::stack<wxWindow*> windowStack;
		windowStack.push(this);

		bool veto = false;

		while (!windowStack.empty()) {
			wxWindow* currentWindow = windowStack.top();
			windowStack.pop();

			wxWindowList& items = currentWindow->GetChildren();
			for (auto item : items) {
				windowStack.push(item);

				if ((CheckItem<CloseType>(item) || ...))veto = true;
			}
		}
		return veto;
	}
};

//Overrides the On Close event handler to ask all children to Close and if the return true to veto
template<IsCloseType... CloseType>
	requires (sizeof...(CloseType) > 0)
class CloseChildrenFramHelper{
private:
	template<IsCloseType CheckCloseType>
	bool CheckItem(wxWindowList::iterator::value_type item) {
		CloseableChildHelper<CheckCloseType>* closable = dynamic_cast<CloseableChildHelper<CheckCloseType>*>(item);
		return closable && closable->TryCloseVeto();
	}

	wxFrame* Frame;

protected:
	void OnClose(wxCloseEvent& evt) {
		if (!evt.CanVeto() || !Frame) {
			evt.Skip();
			return;
		}

		std::stack<wxWindow*> windowStack;
		windowStack.push(Frame);

		bool veto = false;

		while (!windowStack.empty()) {
			wxWindow* currentWindow = windowStack.top();
			windowStack.pop();

			wxWindowList& items = currentWindow->GetChildren();
			for (auto item : items) {
				windowStack.push(item);

				if ((CheckItem<CloseType>(item)||...))veto = true;
			}
		}
		if (veto) {
			evt.Veto();
		}
		else {
			if (Frame) {
				Frame->Destroy();
			}
		}
	}

	CloseChildrenFramHelper(wxFrame* Frame) :Frame(Frame){
		Frame->Bind(wxEVT_CLOSE_WINDOW, &CloseChildrenFramHelper::OnClose, this);
	}
};

//Beispiel:
//
//DECLARE_CLOSE_TYPE(Button);
//DECLARE_CLOSE_TYPE(Other);
//
//class MyWindow : public wxWindow, public CloseableChildHelper<CloseType_Other> {
//	bool TryCloseVeto()override {
//		wxMessageBox("Arent Ready to close");
//		return true;
//	}
//public:
//	MyWindow(wxWindow* parent, wxWindowID id = wxID_ANY) :wxWindow(parent, id) {
//
//	}
//};
//
//class MyFrame_1 :public CloseChildrenFramHelper<CloseType_Button> {
//public:
//	MyFrame_1() :CloseChildrenFramHelper(nullptr,wxID_ANY, "Title") {
//		//auto* sizer = new wxBoxSizer(wxVERTICAL);
//		MyWindow* window = new MyWindow(this);
//		//sizer->Add(window, 1, wxEXPAND);
//		//SetSizerAndFit(sizer);
//	}
//
//};
