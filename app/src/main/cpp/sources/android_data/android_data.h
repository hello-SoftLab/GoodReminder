#pragma once


#include "global.h"
#include "SDL.h"

class AndroidData {
public:

    static void Init();

    static ecspp::HelperClasses::FunctionSink<void()> onDestroy();
    static ecspp::HelperClasses::FunctionSink<void()> onLowMemory();
    static ecspp::HelperClasses::FunctionSink<void()> onPause();
    static ecspp::HelperClasses::FunctionSink<void()> onResume();
    static ecspp::HelperClasses::FunctionSink<void(int,int)> onResize();
    static ecspp::HelperClasses::FunctionSink<void(SDL_Event*)> onFingerDown();
    static ecspp::HelperClasses::FunctionSink<void(SDL_MultiGestureEvent)> onMultiGesture();

    static float GetKeyboardHeight();
    static ImVec2 GetMonitorSize();


    static ImGuiIO& IO();
    static SDL_Window* CurrentWindow();

private:

    static int SDLCALL EventFilter(void* userData,SDL_Event* event);
    static bool InitializeImGui();
    static bool InitializeContext();
    static bool InitializeWindow();


    static void ShutDown();



    static inline ecspp::HelperClasses::EventLauncher<void(SDL_Event*)> m_FingerDownEvent;
    static inline ecspp::HelperClasses::EventLauncher<void(SDL_MultiGestureEvent)> m_MultiGestureEvent;
    static inline ecspp::HelperClasses::EventLauncher<void(int,int)> m_ResizeEvent;
    static inline ecspp::HelperClasses::EventLauncher<void()> m_Terminating;
    static inline ecspp::HelperClasses::EventLauncher<void()> m_LowMemoryEvent;
    static inline ecspp::HelperClasses::EventLauncher<void()> m_DidEnterBgEvent;
    static inline ecspp::HelperClasses::EventLauncher<void()> m_DidEnterFgEvent;



    static inline std::string m_GLSLVersion = "#version 300 es";
    static inline SDL_DisplayMode m_DisplayProperties;
    static inline ImGuiIO* m_IO = nullptr;
    static inline SDL_Window* m_WindowPointer = nullptr;
    static inline SDL_GLContext m_Context;



};




