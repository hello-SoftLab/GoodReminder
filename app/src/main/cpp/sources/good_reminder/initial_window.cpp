#include "initial_window.h"
#include "../global.h"
#include "../android_data/android_data.h"
#include "../app_manager/app_manager.h"

void InitialWindow::Update() {

    if(m_Properties.animationState == AnimationState::InitialAnimation){
        std::string logoText = "Good Reminder";
        ImGui::SetCursorPos(ImVec2(AndroidData::GetMonitorSize().x/2 - ImGui::CalcTextSize(logoText.c_str()).x/2,AndroidData::GetMonitorSize().y/2 - ImGui::CalcTextSize(logoText.c_str()).y/2));
        float scale = 1;
        if(m_Properties.animationTime > 5){
            scale = ((m_Properties.animationTime - 4)*10);
            if(scale < 0.01) {
                m_Properties.animationState == AnimationState::NoAnimation;
            }

        }
        ImGui::SetWindowFontScale(2 / scale);
        ImGui::Text("%s", logoText.c_str());

    }



    m_Properties.animationTime += AppManager::DeltaTime();

}

void InitialWindow::Init() {


    AppManager::SetClearColor(Color(255,221,166)); //
}

ecspp::HelperClasses::FunctionSink<void()> InitialWindow::OnAnimationFinish() {
    return {m_OnAnimationFinish};
}
