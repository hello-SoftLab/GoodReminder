#pragma once

#include <app_manager/app_manager.h>

#include "global.h"

class AndroidData {
public:
    static struct android_app* App();
    static void Init(struct android_app* app);
    static void MainLoop(struct android_app* app);
    static bool OpenSoftKeyboard();
    static bool CloseSoftKeyboard();
    static bool IsSoftKeyboardOpen();
    static JNIEnv* Env();
    static JavaVM* VM();
    static ImGuiIO& IO();
    static void SwapBuffers();

private:
    static bool IsSoftKeyboardOpenCommand();

    static void ShutDown();


    static void HandleAppCommands(struct android_app* app,int32_t appCmd);
    static int32_t HandleInputEvent(struct android_app* app,AInputEvent* inputEvent);

    static inline bool m_IsKeyboardOpen = false;
    static inline bool g_Initialized = false;
    static inline ImGuiIO* m_IO = nullptr;
    static inline struct android_app* g_App = nullptr;
    static inline EGLDisplay g_EglDisplay = EGL_NO_DISPLAY;
    static inline EGLSurface g_EglSurface = EGL_NO_SURFACE;
    static inline EGLContext g_EglContext = EGL_NO_CONTEXT;
    static inline char g_LogTag[] = "ImGuiExample";


};




