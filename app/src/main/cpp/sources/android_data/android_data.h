#pragma once

#include <app_manager/app_manager.h>

#include "global.h"
#include "SDL.h"

class AndroidData {
public:

    static void Init();


    static ImGuiIO& IO();
    static SDL_Window* CurrentWindow();

private:
    static bool InitializeImGui();
    static bool InitializeContext();
    static bool InitializeWindow();


    static void ShutDown();



    static inline std::string m_GLSLVersion = "#version 300 es";
    static inline SDL_DisplayMode m_DisplayProperties;
    static inline ImGuiIO* m_IO = nullptr;
    static inline SDL_Window* m_WindowPointer = nullptr;
    static inline SDL_GLContext m_Context;



};




