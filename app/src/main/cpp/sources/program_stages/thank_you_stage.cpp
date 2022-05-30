#include "thank_you_stage.h"
#include "../android_data/android_data.h"
#include "loading_stage.h"

void ThankYouStage::Update(float deltaTime) {

    std::string data = "thank you";
    if(counter > 4){
        InitialWindow::SetProgramStage<LoadingStage>();
        return;
    }

    ImGui::SetCursorPosX(AndroidData::GetMonitorSize().x/2 - ImGui::CalcTextSize(data.c_str()).x/2);
    ImGui::SetCursorPosY(AndroidData::GetMonitorSize().y/2 - ImGui::CalcTextSize(data.c_str()).y/2);


    if(counter > 3 && counter < 4) {
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha,4-counter);
    }
    if(counter < 1){
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha,counter);
    }

    ImGui::Text("%s",data.c_str());

    if((counter > 3 && counter < 4) || counter < 1){
        ImGui::PopStyleVar();
    }


    counter += deltaTime;

}

void ThankYouStage::Init() {
    connectionID = AndroidData::onFingerEvent().Connect([](SDL_Event* ev){
        InitialWindow::SetProgramStage<LoadingStage>();
    });
}

ThankYouStage::~ThankYouStage() {
    AndroidData::onFingerEvent().Disconnect(connectionID);
}
