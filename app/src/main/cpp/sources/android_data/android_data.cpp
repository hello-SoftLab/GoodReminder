#include "android_data.h"
#include "../app_manager/app_manager.h"

void AndroidData::Init() {
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        return;
    }

    if(!InitializeWindow()){
        return;
    }

    if(!InitializeContext()){
        return;
    }

    if(!InitializeImGui()){
        return;
    }



    while(AppManager::HandleFrameUpdate()){

    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(m_Context);
    // Close and destroy the window
    SDL_DestroyWindow(m_WindowPointer);

    // Clean up
    SDL_Quit();


}

ImGuiIO &AndroidData::IO() {
    return *m_IO;
}

bool AndroidData::InitializeContext() {

    m_Context = SDL_GL_CreateContext(m_WindowPointer);

    if(m_Context == nullptr){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error","OpenGL ES3 not supported on device",m_WindowPointer);
        return false;
    }

    SDL_GL_MakeCurrent(m_WindowPointer, m_Context);

    // enable VSync
    SDL_GL_SetSwapInterval(1);
    
    return true;
}

bool AndroidData::InitializeWindow() {

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);



    SDL_GetCurrentDisplayMode(0,&m_DisplayProperties);



    m_WindowPointer = SDL_CreateWindow(
            "",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            m_DisplayProperties.w,
            m_DisplayProperties.h,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED
    );

    // Check that the window was successfully created
    if (m_WindowPointer == NULL) {
        // In the case that the window could not be made...
        std::string error = SDL_GetError();
        return false;
    }

    return true;
}

void AndroidData::ShutDown() {

}

bool AndroidData::InitializeImGui() {

    IMGUI_CHECKVERSION();

    if(!ImGui::CreateContext()){
        return false;
    }
    m_IO = &ImGui::GetIO();

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForOpenGL(m_WindowPointer,m_Context);
    ImGui_ImplOpenGL3_Init(m_GLSLVersion.c_str());

    return true;
}

SDL_Window *AndroidData::CurrentWindow() {
    return m_WindowPointer;
}
