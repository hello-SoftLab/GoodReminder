#include "loading_stage.h"
#include <fstream>
#include "../android_data/android_data.h"
#include "calendar_stage.h"

void LoadingStage::Init() {

    m_Texture = LoadTextureFromFile<Type2D>(AndroidData::GetDataDir() + "/assets/images/logo-LOADINGPAGEOFF.png");

}

void LoadingStage::Update(float deltaTime) {
    if(m_Texture){
        if(animationTime < 1.5){
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha,animationTime/1.5f);
        }

        ImGui::SetCursorPosX(AndroidData::GetMonitorSize().x/2 - ImGui::GetFontSize()*5);
        ImGui::SetCursorPosY(AndroidData::GetMonitorSize().y/2 - ImGui::GetFontSize()*4);
        ImGui::Image((void*)m_Texture.GetID(),ImVec2(ImGui::GetFontSize()*10,ImGui::GetFontSize()*8));
        if(animationTime < 1.5) {
            ImGui::PopStyleVar();
        }
        if(animationTime > 2.5) {
            InitialWindow::SetProgramStage<CalendarStage>();
        }

    }

    animationTime += deltaTime;

}
