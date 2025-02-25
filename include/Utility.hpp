#pragma once

#include "pch.hpp"

wxDECLARE_EVENT(wxEVT_FIT_REQUEST, wxCommandEvent);
wxDECLARE_EVENT(wxEVT_LAYOUT_REQUEST, wxCommandEvent);
wxDECLARE_EVENT(wxEVT_ADJUST_REQUEST, wxCommandEvent);

using FunktionForAllChildren = std::function<void(wxWindow*)>;

extern void ForAllChildren(wxWindow* w, FunktionForAllChildren f);

class TransmitHideShowFromCollapsible : public wxCollapsiblePane {
public:
    TransmitHideShowFromCollapsible(wxWindow* Parent, wxWindowID id, const wxString& lable, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 2621440L, const wxValidator& val = wxDefaultValidator, const wxString& name = wxString::FromAscii("wxCollapsiblePaneString"));
};

class ReciveHideShowFromCollapsible {
public:

    //To BeOverriden
    virtual wxWindow* GetParent() = 0;
    bool IsCollapsiblePaneCollapsed()
    {
        wxWindow* parent = GetParent();
        while (parent)
        {
            wxCollapsiblePane* collapsiblePane = dynamic_cast<wxCollapsiblePane*>(parent);
            if (collapsiblePane)
            {
                return collapsiblePane->IsCollapsed();
            }
            parent = parent->GetParent();
        }
        return false; // Wenn kein wxCollapsiblePane gefunden wurde, nehmen wir an, dass es nicht eingeklappt ist
    }
    //Check 
    virtual void OnCollapsible() = 0;
};

class TempDirManager {
public:
    std::filesystem::path TempDir = "Temp";
    TempDirManager();
    ~TempDirManager();
};

//extern TempDirManager Tempdirmgr;
