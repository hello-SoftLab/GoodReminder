#include "app_manager.h"
#include "../android_data/android_data.h"
#include "../helpers/color.h"



void AppManager::BeginFrame() {
    static bool wantedTextInputLastFrame = false;
    static float lastTime,currentTime;


    currentTime = SDL_GetTicks()/1000.0f;

    m_DeltaTime = currentTime - lastTime;

    lastTime = currentTime;


    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();




    if(AndroidData::IO().WantTextInput){
            SDL_StartTextInput();
    }
    else{
        SDL_StopTextInput();
    }

    wantedTextInputLastFrame = AndroidData::IO().WantTextInput;


}

void AppManager::Draw() {

    ImGui::SetNextWindowPos(ImVec2(0,0));
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x,ImGui::GetIO().DisplaySize.y));

    if(ImGui::Begin("Hello!",0,ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground)){


        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding,10);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding,10);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding,10);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize,4);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,Color::AsNormals(0.26f, 0.59f, 0.98f, 0.40f).AsImVec4());
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,Color::AsNormals(0.26f, 0.59f, 0.98f, 0.40f).AsImVec4());

        m_DrawingFunctions.EmitEvent();

        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar(4);


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
    GL_CALL(glClearColor(m_ClearColor.Normalized().x,m_ClearColor.Normalized().y,m_ClearColor.Normalized().z,1));
    GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
}

bool AppManager::HandleFrameUpdate() {

    static ImVec2 oldMousePos;

    m_MouseDelta.x = ImGui::GetMousePos().x - oldMousePos.x;
    m_MouseDelta.y = ImGui::GetMousePos().y - oldMousePos.y;

    oldMousePos = ImGui::GetMousePos();

    BeginFrame();

    Draw();

    EndFrame();

    return true;

}

yael::event_sink<void()> AppManager::Drawing() {
    return {m_DrawingFunctions};
}

void AppManager::SetClearColor(Color color) {
    m_ClearColor = color;
}

float AppManager::DeltaTime() {
    return m_DeltaTime;
}



