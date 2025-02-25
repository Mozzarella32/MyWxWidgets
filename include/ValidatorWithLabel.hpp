#pragma once

#include "pch.hpp"

class ValidatorWithLabel : public wxValidator
{
public:
    ValidatorWithLabel(wxStaticText* msgLabel, wxString* val = nullptr);

    ValidatorWithLabel(const ValidatorWithLabel& validator);

    virtual ~ValidatorWithLabel() {}

    virtual wxObject* Clone() const override = 0;

    bool TransferFromWindow() override;

    bool TransferToWindow() override;

    virtual bool Validate(wxWindow* parent) override;

    void UpdateValidationLabel();

protected:
    virtual std::pair<bool, wxString> CheckValidity(const wxString& str) const = 0;

    virtual void SetupEvents();
    virtual void OnText(wxCommandEvent& e);

    wxTextEntry* GetTextEntry() const;

private:
    wxStaticText* validationMsgLabel;
    wxString* value;
};
