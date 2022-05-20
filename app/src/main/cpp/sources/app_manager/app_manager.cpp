#include "app_manager.h"
#include "../android_data/android_data.h"
#include "../helpers/color.h"



void AppManager::BeginFrame() {


    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();




    if(AndroidData::IO().WantTextInput){
        SDL_StartTextInput();
    }
    else {
        SDL_StopTextInput();
    }

}

void AppManager::Draw() {

    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x,ImGui::GetIO().DisplaySize.y));
    if(ImGui::Begin("Hello!",0,ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar)){
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding,10);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding,10);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,10);

        m_DrawingFunctions.EmitEvent();

        ImGui::PopStyleVar(3);


        ImGui::ScrollWhenDraggingOnVoid(ImVec2(0.0f,-AndroidData::IO().MouseDelta.y),ImGuiMouseButton_Left);
    }

    ImGui::End();
}

void AppManager::EndFrame() {
    //ImGui::ShowDemoWindow();

    ImGuiIO& io = ImGui::GetIO();

    ImGui::Render();
    GL_CALL(glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y));
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(AndroidData::CurrentWindow());
    GL_CALL(glClearColor(0.45f, 0.55f, 0.60f,1));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
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

HelperClasses::FunctionSink<void()> AppManager::Drawing() {
    return {m_DrawingFunctions};
}

void ImGui::ScrollWhenDraggingOnVoid(const ImVec2 &delta, ImGuiMouseButton mouse_button) {

    ImGuiContext& g = *ImGui::GetCurrentContext();
    ImGuiWindow* window = g.CurrentWindow;
    bool hovered = false;
    bool held = false;
    ImGuiButtonFlags button_flags = (mouse_button == 0) ? ImGuiButtonFlags_MouseButtonLeft : (mouse_button == 1) ? ImGuiButtonFlags_MouseButtonRight : ImGuiButtonFlags_MouseButtonMiddle;
    if (g.HoveredId == 0) // If nothing hovered so far in the frame (not same as IsAnyItemHovered()!)
        ImGui::ButtonBehavior(window->Rect(), window->GetID("##scrolldraggingoverlay"), &hovered, &held, button_flags);
    if (held && delta.x != 0.0f)
        ImGui::SetScrollX(window, window->Scroll.x + delta.x);
    if (held && delta.y != 0.0f)
        ImGui::SetScrollY(window, window->Scroll.y + delta.y);
}
