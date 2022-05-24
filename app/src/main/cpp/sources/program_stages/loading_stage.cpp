#include "loading_stage.h"
#include <fstream>
#include "../android_data/android_data.h"


void LoadingStage::Init() {

    m_Texture = AndroidData::GetLoadedTexture( "logo-ALLLETTERCASEOFF.png");

}

void LoadingStage::Update(float deltaTime) {


    if(m_Texture){
        ImGui::Image((void*)m_Texture.GetID(),ImVec2(100,100));

    }

    animationTime += deltaTime;

}
