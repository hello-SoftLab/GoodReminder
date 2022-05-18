#include "app_manager.h"
#include "../android_data/android_data.h"

void AppManager::HandleFrameUpdate() {
    BeginFrame();
    Draw();
    EndFrame();
}

void AppManager::BeginFrame() {

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();


}

void AppManager::Draw() {
    static std::string data = "";
    static bool isTextShown = false;
    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x,ImGui::GetIO().DisplaySize.y));
    if(ImGui::Begin("Hello!",0,ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar)){


        if(ImGui::InputTextMultiline("##data",&data,ImVec2(AndroidData::IO().DisplaySize.x,AndroidData::IO().DisplaySize.y/3))){

        }
        if(ImGui::IsItemActive() && !isTextShown){
            AndroidData::OpenSoftKeyboard();
            isTextShown = true;
        }
        if(!ImGui::IsItemActive() && isTextShown){
            AndroidData::CloseSoftKeyboard();
            isTextShown = false;
        }



        ImGui::SetWindowFontScale(3);
    }

    ImGui::End();
}

void AppManager::EndFrame() {
    ImGuiIO& io = ImGui::GetIO();

    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    AndroidData::SwapBuffers();
}