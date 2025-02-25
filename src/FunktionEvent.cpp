#include "FunktionEvent.hpp"

IdleFunktionEvent::IdleFunktionEvent(wxEventType Type, std::function<void(void)> func)
    : wxEvent(wxID_ANY, Type), m_function(func) {}

std::function<void(void)> IdleFunktionEvent::GetFunction() const { return m_function; }

wxEvent* IdleFunktionEvent::Clone() const { return new IdleFunktionEvent(*this); }

wxDEFINE_EVENT(wxEVT_ADD_TO_IDLE_QUEUE, IdleFunktionEvent);
//wxDEFINE_EVENT(wxEVT_STOPED_IDLE_QUEUE, IdleFunktionEvent);
