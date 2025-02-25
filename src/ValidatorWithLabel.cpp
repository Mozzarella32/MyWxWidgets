#include "ValidatorWithLabel.hpp"

ValidatorWithLabel::ValidatorWithLabel(wxStaticText* msgLabel, wxString* val)
    : value(val), validationMsgLabel(msgLabel)
{
    validationMsgLabel->SetDoubleBuffered(true);
    SetupEvents();
}

ValidatorWithLabel::ValidatorWithLabel(const ValidatorWithLabel& validator)
    : wxValidator(validator)
{
    value = validator.value;
    validationMsgLabel = validator.validationMsgLabel;

    SetupEvents(); // this is why we need copy constructor
}

bool ValidatorWithLabel::TransferFromWindow()
{
    if (value)
    {
        wxTextEntry* const text = GetTextEntry();
        *value = text->GetValue();
    }

    return true;
}

bool ValidatorWithLabel::TransferToWindow()
{
    if (value)
    {
        wxTextEntry* const text = GetTextEntry();
        text->SetValue(*value);
    }

    return true;
}

bool ValidatorWithLabel::Validate(wxWindow* parent)
{
    const auto& [valid, _] = CheckValidity(GetTextEntry()->GetValue());
    return valid;
}

void ValidatorWithLabel::UpdateValidationLabel()
{
    auto [valid, text] = CheckValidity(GetTextEntry()->GetValue());

    validationMsgLabel->SetLabel(text);
    validationMsgLabel->SetForegroundColour(
        valid ? wxColor(100, 200, 100) : wxColor(200, 100, 100));

    validationMsgLabel->Wrap(validationMsgLabel->GetMaxWidth());

    validationMsgLabel->Layout();
    validationMsgLabel->Refresh();
    validationMsgLabel->GetParent()->Fit();
    validationMsgLabel->GetParent()->Refresh(); // needed for Windows to update the color
}

void ValidatorWithLabel::SetupEvents() { Bind(wxEVT_TEXT, &ValidatorWithLabel::OnText, this); }

void ValidatorWithLabel::OnText(wxCommandEvent& e)
{
    UpdateValidationLabel();
    e.Skip();
}

wxTextEntry* ValidatorWithLabel::GetTextEntry() const { return dynamic_cast<wxTextEntry*>(GetWindow()); }
