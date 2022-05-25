#include "loading_stage.h"
#include <fstream>
#include "../android_data/android_data.h"
#include "calendar_stage.h"

void LoadingStage::Init() {

    m_Texture = AndroidData::GetLoadedTexture( "logo/logo-ALLLETTERCASEOFF.png");

}

void LoadingStage::Update(float deltaTime) {



    if(m_Texture){
        if(animationTime < 1.5){
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha,animationTime/1.5f);
        }

        ImGui::SetCursorPosX(AndroidData::GetMonitorSize().x/2 - m_Texture.GetWidth()/10);
        ImGui::SetCursorPosY(AndroidData::GetMonitorSize().y/2 - m_Texture.GetHeight()/10);
        ImGui::Image((void*)m_Texture.GetID(),ImVec2(m_Texture.GetWidth()/5,m_Texture.GetHeight()/5));
        if(animationTime < 1.5) {
            ImGui::PopStyleVar();
        }
        if(animationTime > 3) {
            InitialWindow::SetProgramStage<CalendarStage>();
        }

    }

    animationTime += deltaTime;

}
