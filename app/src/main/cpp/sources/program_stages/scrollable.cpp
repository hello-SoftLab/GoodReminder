#include "scrollable.h"

Scrollable::Scrollable() {
    m_ConnectionID = AndroidData::onFingerEvent().Connect([&](SDL_Event* event){
        if(event->tfinger.type == SDL_FINGERMOTION){
            m_ScrollAccel = event->tfinger.dy*AndroidData::GetMonitorSize().y;
            m_ShouldScroll = true;
        }

    });
}

void Scrollable::HandleScroll() {
    if(m_ShouldScroll){
        ImGui::SetScrollY(ImGui::GetScrollY() - m_ScrollAccel);

        if(m_ScrollAccel > 0) {
            m_ScrollAccel -= m_ScrollFriction * AppManager::DeltaTime();
        }
        if(m_ScrollAccel < 0) {
            m_ScrollAccel += m_ScrollFriction * AppManager::DeltaTime();
        }

        if(abs(m_ScrollAccel) < 1){
            m_ShouldScroll = false;
        }
        if(ImGui::GetScrollY() == ImGui::GetScrollMaxY()){
            m_ShouldScroll = false;
        }
    }
}

Scrollable::~Scrollable() {
    AndroidData::onFingerEvent().Disconnect(m_ConnectionID);
}
