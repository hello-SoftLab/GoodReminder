#pragma once
#include "global.h"
#include "SDL.h"
#include "../opengl_wrappers/texture.h"

struct LoadedFileContents {
    int width = -1;
    int height = -1;
    std::vector<unsigned char> data;
};

struct DelayedFunctionStructure {
    std::function<void()> m_Function;
    float currentTimer = 0;
    float maxTime = 0;
};

class AndroidData {
public:

    static void Init();
    static void Initialization();
    static void Cleanup();

    static yael::event_sink<void(SDL_Event*)> onKeyboard();
    static yael::event_sink<void()> onDestroy();
    static yael::event_sink<void()> onLowMemory();
    static yael::event_sink<void()> onPause();
    static yael::event_sink<void()> onResume();
    static yael::event_sink<void(int,int)> onResize();
    static yael::event_sink<void(SDL_Event*)> onFingerEvent();
    static yael::event_sink<void(SDL_MultiGestureEvent)> onMultiGesture();

    static ImVec2 GetDragDelta();

    static Texture<Type2D> GetLoadedTexture(std::string path);

    static void LoadImage(std::string name);
    static void SetImageToBeLoaded(std::string name,std::vector<unsigned char> data,int width,int height);
    static void SetDataDir(std::string name);
    static std::string GetDataDir();

    static float GetKeyboardHeight();
    static ImVec2 GetMonitorSize();


    static ImGuiIO& IO();
    static SDL_Window* CurrentWindow();

    static void TimedFunctionCall(float delay,std::function<void()> func);

private:

    static int SDLCALL EventFilter(void* userData,SDL_Event* event);
    static bool InitializeImGui();
    static bool InitializeContext();
    static bool InitializeWindow();


    static void ShutDown();

    static inline bool m_IsRunning = true;
    static inline yael::event_launcher<void(SDL_Event*)> m_KeyboardEvent;
    static inline yael::event_launcher<void(SDL_Event*)> m_FingerDownEvent;
    static inline yael::event_launcher<void(SDL_MultiGestureEvent)> m_MultiGestureEvent;
    static inline yael::event_launcher<void(int,int)> m_ResizeEvent;
    static inline yael::event_launcher<void()> m_Terminating;
    static inline yael::event_launcher<void()> m_LowMemoryEvent;
    static inline yael::event_launcher<void()> m_DidEnterBgEvent;
    static inline yael::event_launcher<void()> m_DidEnterFgEvent;

    static inline std::string m_DataDir;
    static inline std::vector<DelayedFunctionStructure> m_DelayedFunctions;
    static inline SDL_Rect m_Dimensions;
    static inline ImVec2 m_DragDelta;
    static inline std::unordered_map<std::string,LoadedFileContents> m_ImagesToBeLoaded;
    static inline std::unordered_map<std::string,Texture<Type2D>> m_LoadedImages;
    static inline std::string m_GLSLVersion = "#version 300 es";
    static inline SDL_DisplayMode m_DisplayProperties;
    static inline ImGuiIO* m_IO = nullptr;
    static inline SDL_Window* m_WindowPointer = nullptr;
    static inline SDL_GLContext m_Context;



};




