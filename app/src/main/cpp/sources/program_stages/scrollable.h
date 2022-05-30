#pragma once
#include "../good_reminder/initial_window.h"
#include "../android_data/android_data.h"
#include "../app_manager/app_manager.h"

class Scrollable {
public:
    Scrollable();

    ~Scrollable();

    void HandleScroll();

private:
    bool m_ShouldScroll = false;
    float m_ScrollValue = 0;
    float m_ScrollFriction = 40;
    float m_ScrollAccel;
    size_t m_ConnectionID = 0;

};