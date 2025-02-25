#pragma once

#include "pch.hpp"

class IdleFunktionEvent;

wxDECLARE_EVENT(wxEVT_ADD_TO_IDLE_QUEUE, IdleFunktionEvent);
//wxDECLARE_EVENT(wxEVT_STOPED_IDLE_QUEUE, IdleFunktionEvent);

class IdleFunktionEvent : public wxEvent
{
public:
    IdleFunktionEvent(wxEventType Type, std::function<void(void)> func);

    std::function<void(void)> GetFunction() const;

    virtual wxEvent* Clone() const override;

private:
    std::function<void(void)> m_function;
};

inline void IdleCallAfter(const std::function<void(void)>& fn) {
    IdleFunktionEvent evt(wxEVT_ADD_TO_IDLE_QUEUE,fn);
    wxTheApp->ProcessEvent(evt);
}

inline void SynchronIdleCallAfter(const std::function<void(void)>& fn) {
    bool Finished = false;
    std::condition_variable cv;
    std::mutex cv_mutex;

    auto fnSync = [&fn, &cv, &Finished]() {
        fn();
        Finished = true;
        cv.notify_all();
        };

    IdleFunktionEvent evt(wxEVT_ADD_TO_IDLE_QUEUE,fnSync);
    wxTheApp->ProcessEvent(evt);

    std::unique_lock ul_cv(cv_mutex);
    cv.wait(ul_cv, [&Finished]() {
        return Finished;
        });
}
//
//inline void SynchronCallAfterWithStopedIdleQueue(const std::function<void(void)>& fn) {
//    bool Finished = false;
//    std::condition_variable cv;
//    std::mutex cv_mutex;
//
//    auto fnSync = [&fn, &cv, &Finished]() {
//        fn();
//        Finished = true;
//        cv.notify_all();
//        };
//
//    IdleFunktionEvent evt(wxEVT_STOPED_IDLE_QUEUE,fnSync);
//    wxTheApp->ProcessEvent(evt);
//
//    std::unique_lock ul_cv(cv_mutex);
//    cv.wait(ul_cv, [&Finished]() {
//        return Finished;
//        });
//}