#include "loading_stage.h"
#include <fstream>

void LoadingStage::Init() {
    std::string path = "";


    path += "data/data/knz.goodreminder/";
    path += "logo_alletercaseoff.png";
    //m_Texture = LoadTextureFromFile<Type2D>( path);

    if(!std::filesystem::exists(path)){
        std::fstream stream;
        stream.open(path);
        stream << "i";
        stream.close();
    }

}

void LoadingStage::Update(float deltaTime) {


    if(m_Texture){
        ImGui::Image((void*)m_Texture.GetID(),ImVec2(100,100));

    }

    animationTime += deltaTime;

}
