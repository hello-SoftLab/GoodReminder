#include "app_manager.h"
#include "../android_data/android_data.h"
#include "../helpers/color.h"



void AppManager::BeginFrame() {

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT);


    if(AndroidData::IO().WantCaptureKeyboard){
        SDL_StartTextInput();
    }
    else {
        SDL_StopTextInput();
    }


    ImGui::ShowDemoWindow();

}

void AppManager::Draw() {
    static std::string data = "";
    static bool isTextShown = false;
    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x,ImGui::GetIO().DisplaySize.y));
    if(ImGui::Begin("Hello!",0,ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar)){


        if(ImGui::InputTextMultiline("##data",&data,ImVec2(AndroidData::IO().DisplaySize.x,AndroidData::IO().DisplaySize.y/3))){

        }



        ImGui::SetWindowFontScale(3);
    }

    ImGui::End();
}

void AppManager::EndFrame() {
    ImGuiIO& io = ImGui::GetIO();

    ImGui::Render();
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(AndroidData::CurrentWindow());
}

bool AppManager::HandleFrameUpdate() {
    SDL_Event event;


    while(SDL_PollEvent(&event)){
        ImGui_ImplSDL2_ProcessEvent(&event);
        switch(event.type){
            case SDL_QUIT:
                return false;
            case SDL_WINDOWEVENT:
                if(event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(AndroidData::CurrentWindow())){
                    return false;
                }
                break;
        }
    }

    BeginFrame();

    Draw();

    EndFrame();

    return true;

}
