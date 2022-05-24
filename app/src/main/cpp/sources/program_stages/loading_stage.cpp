#include "loading_stage.h"
#include <fstream>
#include "../android_data/android_data.h"


void LoadingStage::Init() {

    m_Texture = AndroidData::GetLoadedTexture( "logo/logo-ALLLETTERCASEOFF.png");

}

void LoadingStage::Update(float deltaTime) {



    if(m_Texture){
        ImGui::SetCursorPosX(AndroidData::GetMonitorSize().x/2 - m_Texture.GetWidth()/20);
        ImGui::SetCursorPosY(AndroidData::GetMonitorSize().y/2 - m_Texture.GetHeight()/20);
        ImGui::Image((void*)m_Texture.GetID(),ImVec2(m_Texture.GetWidth()/10,m_Texture.GetHeight()/10));

    }

    animationTime += deltaTime;

}
