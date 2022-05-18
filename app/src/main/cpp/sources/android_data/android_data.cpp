#include "android_data.h"
#include "../app_manager/app_manager.h"

android_app *AndroidData::App() {
    return g_App;
}

void AndroidData::Init(struct android_app *data) {


    //initializing
    ANativeWindow_acquire(g_App->window);
    {
        g_EglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (g_EglDisplay == EGL_NO_DISPLAY)
            __android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s", "eglGetDisplay(EGL_DEFAULT_DISPLAY) returned EGL_NO_DISPLAY");

        if (eglInitialize(g_EglDisplay, 0, 0) != EGL_TRUE)
            __android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s", "eglInitialize() returned with an error");

        const EGLint egl_attributes[] = { EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_RED_SIZE, 8, EGL_DEPTH_SIZE, 24, EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_NONE };
        EGLint num_configs = 0;
        if (eglChooseConfig(g_EglDisplay, egl_attributes, nullptr, 0, &num_configs) != EGL_TRUE)
            __android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s", "eglChooseConfig() returned with an error");
        if (num_configs == 0)
            __android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s", "eglChooseConfig() returned 0 matching config");

        // Get the first matching config
        EGLConfig egl_config;
        eglChooseConfig(g_EglDisplay, egl_attributes, &egl_config, 1, &num_configs);
        EGLint egl_format;
        eglGetConfigAttrib(g_EglDisplay, egl_config, EGL_NATIVE_VISUAL_ID, &egl_format);
        ANativeWindow_setBuffersGeometry(g_App->window, 0, 0, egl_format);

        const EGLint egl_context_attributes[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
        g_EglContext = eglCreateContext(g_EglDisplay, egl_config, EGL_NO_CONTEXT, egl_context_attributes);

        if (g_EglContext == EGL_NO_CONTEXT) {
            __android_log_print(ANDROID_LOG_ERROR, g_LogTag, "%s",
                                "eglCreateContext() returned EGL_NO_CONTEXT");
            ShutDown();
        }

        g_EglSurface = eglCreateWindowSurface(g_EglDisplay, egl_config, g_App->window, nullptr);
        eglMakeCurrent(g_EglDisplay, g_EglSurface, g_EglSurface, g_EglContext);
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();

    m_IO = &io;
    // Disable loading/saving of .ini file from disk.
    // FIXME: Consider using LoadIniSettingsFromMemory() / SaveIniSettingsToMemory() to save in appropriate location for Android.
    io.IniFilename = nullptr;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplAndroid_Init(g_App->window);
    ImGui_ImplOpenGL3_Init("#version 300 es");




    g_Initialized = true;
}

void AndroidData::HandleAppCommands(struct android_app *app, int32_t appCmd) {
    switch (appCmd)
    {
        case APP_CMD_SAVE_STATE:
            break;
        case APP_CMD_INIT_WINDOW:
            Init(app);
            break;
        case APP_CMD_TERM_WINDOW:
            ShutDown();
            break;
        case APP_CMD_GAINED_FOCUS:
            break;
        case APP_CMD_LOST_FOCUS:
            break;
    }
}

int32_t AndroidData::HandleInputEvent(struct android_app *app,AInputEvent* inputEvent) {
    return ImGui_ImplAndroid_HandleInputEvent(inputEvent);
}

void AndroidData::MainLoop(struct android_app* app) {
    if(!g_App) {
        g_App = app;
    }
    g_App->onAppCmd = HandleAppCommands;
    g_App->onInputEvent = HandleInputEvent;

    while (true) {
        int out_events;
        struct android_poll_source *out_data;

        // Poll all events. If the app is not visible, this loop blocks until g_Initialized == true.
        while (ALooper_pollAll(g_Initialized ? 0 : -1, nullptr, &out_events, (void **) &out_data) >=
               0) {
            // Process one event
            if (out_data != nullptr)
                out_data->process(g_App, out_data);

            // Exit the app by returning from within the infinite loop
            if (g_App->destroyRequested != 0) {
                // shutdown() should have been called already while processing the
                // app command APP_CMD_TERM_WINDOW. But we play save here
                if (!g_Initialized)
                    ShutDown();

                return;
            }
        }

        AppManager::HandleFrameUpdate();
        m_IsKeyboardOpen = IsSoftKeyboardOpenCommand();

    }
}


void AndroidData::ShutDown() {
    if (!g_Initialized)
        return;

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplAndroid_Shutdown();
    ImGui::DestroyContext();

    if (g_EglDisplay != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(g_EglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

        if (g_EglContext != EGL_NO_CONTEXT)
            eglDestroyContext(g_EglDisplay, g_EglContext);

        if (g_EglSurface != EGL_NO_SURFACE)
            eglDestroySurface(g_EglDisplay, g_EglSurface);

        eglTerminate(g_EglDisplay);
    }

    g_EglDisplay = EGL_NO_DISPLAY;
    g_EglContext = EGL_NO_CONTEXT;
    g_EglSurface = EGL_NO_SURFACE;
    ANativeWindow_release(g_App->window);

    g_Initialized = false;
}





JNIEnv *AndroidData::Env() {
    return g_App->activity->env;
}

JavaVM *AndroidData::VM() {
    return g_App->activity->vm;
}

ImGuiIO &AndroidData::IO() {
    return *m_IO;
}

bool AndroidData::OpenSoftKeyboard() {


    JNIEnv* env = AndroidData::Env();
    JavaVM* java_vm = AndroidData::VM();

    java_vm->AttachCurrentThread(&env, nullptr);


    jclass native_activity_clazz = env->GetObjectClass(g_App->activity->clazz);


    jmethodID method_id = env->GetMethodID(native_activity_clazz, "showSoftInput", "()V");
    if(method_id == nullptr){
        return false;
    }

    env->CallVoidMethod(g_App->activity->clazz, method_id);

    java_vm->DetachCurrentThread();


    return true;
}

bool AndroidData::CloseSoftKeyboard() {

    JNIEnv* env = AndroidData::Env();
    JavaVM* java_vm = AndroidData::VM();

    java_vm->AttachCurrentThread(&env, nullptr);


    jclass native_activity_clazz = env->GetObjectClass(g_App->activity->clazz);


    jmethodID method_id = env->GetMethodID(native_activity_clazz, "hideSoftInput", "()V");
    if(method_id == nullptr){
        return false;
    }

    env->CallVoidMethod(g_App->activity->clazz, method_id);

    java_vm->DetachCurrentThread();


    return true;
}

bool AndroidData::IsSoftKeyboardOpenCommand() {
    JNIEnv* env = AndroidData::Env();
    JavaVM* java_vm = AndroidData::VM();

    java_vm->AttachCurrentThread(&env, nullptr);


    jclass native_activity_clazz = env->GetObjectClass(g_App->activity->clazz);


    jmethodID method_id = env->GetMethodID(native_activity_clazz, "isActive", "()Z");
    if(method_id == nullptr){
        java_vm->DetachCurrentThread();
        return false;
    }

    jboolean isActive = env->CallBooleanMethod(g_App->activity->clazz, method_id);

    java_vm->DetachCurrentThread();
    return isActive == JNI_TRUE;

}

bool AndroidData::IsSoftKeyboardOpen() {
    return m_IsKeyboardOpen;
}

void AndroidData::SwapBuffers() {
    eglSwapBuffers(g_EglDisplay, g_EglSurface);
}
