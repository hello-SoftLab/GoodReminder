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

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_GetCurrentDisplayMode(0,&m_DisplayProperties);



    m_WindowPointer = SDL_CreateWindow(
            "",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            m_DisplayProperties.w,
            m_DisplayProperties.h,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    );

    // Check that the window was successfully created
    if (m_WindowPointer == NULL) {
        // In the case that the window could not be made...
        std::string error = SDL_GetError();
        return false;
    }

    SDL_SetEventFilter(&AndroidData::EventFilter,nullptr);

    return true;
}

void AndroidData::ShutDown() {

}

bool AndroidData::InitializeImGui() {

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    m_IO = &ImGui::GetIO();

    ImGui::StyleColorsLight();

    ImGui_ImplSDL2_InitForOpenGL(m_WindowPointer,m_Context);
    ImGui_ImplOpenGL3_Init(m_GLSLVersion.c_str());



    return true;
}

SDL_Window *AndroidData::CurrentWindow() {
    return m_WindowPointer;
}

ecspp::HelperClasses::FunctionSink<void()> AndroidData::onDestroy() {
    return {m_Terminating};
}

ecspp::HelperClasses::FunctionSink<void()> AndroidData::onLowMemory() {
    return {m_LowMemoryEvent};
}

ecspp::HelperClasses::FunctionSink<void()> AndroidData::onPause() {
    return {m_DidEnterBgEvent};
}

ecspp::HelperClasses::FunctionSink<void()> AndroidData::onResume() {
    return {m_DidEnterFgEvent};
}

ecspp::HelperClasses::FunctionSink<void(int,int)> AndroidData::onResize() {
    return {m_ResizeEvent};
}
ecspp::HelperClasses::FunctionSink<void(SDL_Event*)> AndroidData::onFingerDown() {
    return {m_FingerDownEvent};
}
ecspp::HelperClasses::FunctionSink<void(SDL_MultiGestureEvent)> AndroidData::onMultiGesture() {
    return {m_MultiGestureEvent};
}

int AndroidData::EventFilter(void *userData, SDL_Event *event) {

    switch(event->type){
        case SDL_APP_TERMINATING:
            m_Terminating.EmitEvent();
            break;
        case SDL_APP_LOWMEMORY:
            m_LowMemoryEvent.EmitEvent();
            break;
        case SDL_APP_WILLENTERBACKGROUND:
            m_DidEnterBgEvent.EmitEvent();
            break;
        case SDL_APP_WILLENTERFOREGROUND:
            m_DidEnterFgEvent.EmitEvent();
            break;
        case SDL_WINDOWEVENT:
            if(event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED){
                m_ResizeEvent.EmitEvent(event->window.data1,event->window.data2);
            }
            break;
        case SDL_MULTIGESTURE:
            m_MultiGestureEvent.EmitEvent(event->mgesture);
            break;
    }


    return 1;
}

float AndroidData::GetKeyboardHeight() {
    JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();

    // retrieve the Java instance of the SDLActivity
    jobject activity = (jobject)SDL_AndroidGetActivity();

    // find the Java class of the activity. It should be SDLActivity or a subclass of it.
    jclass clazz(env->GetObjectClass(activity));

    // find the identifier of the method to call
    jmethodID method_id = env->GetMethodID(clazz, "getKeyboardScreenHeight", "()F");

    // effectively call the Java method
    jfloat height = env->CallFloatMethod(activity, method_id);

    env->DeleteLocalRef(activity);
    env->DeleteLocalRef(clazz);




    return height;



}

ImVec2 AndroidData::GetMonitorSize() {

    return {static_cast<float>(m_DisplayProperties.w),static_cast<float>(m_DisplayProperties.h)};
}

LoadedFileContents AndroidData::ReadFileBytes(std::string path) {
    LoadedFileContents contents;

    JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();

    jobject activity = (jobject)SDL_AndroidGetActivity();

    jclass clazz(env->GetObjectClass(activity));

    jmethodID method_id = env->GetMethodID(clazz, "loadFileContents", "(Ljava/lang/String)[B");

    jstring stringVal = env->NewStringUTF(path.c_str());

    jobject bitmap = env->CallObjectMethod(activity,method_id,stringVal);

    if(bitmap == nullptr){
        return contents;
    }

    jclass bitmapClass = env->GetObjectClass(bitmap);

    jmethodID gettingWidth = env->GetMethodID(bitmapClass,"getWidth","()I");

    jint width = env->CallIntMethod(bitmap,gettingWidth);

    if(width != 0){

        contents.width = width;

        jmethodID gettingHeight = env->GetMethodID(bitmapClass,"getHeight","()I");

        jint height = env->CallIntMethod(bitmap,gettingHeight);

    }

    jmethodID convertBitmapToByte = env->GetMethodID(clazz,"convertBitmapToByteArray","(Landroid/graphics/Bitmap)[B");

    jbyteArray byteArr = (jbyteArray)env->CallObjectMethod(activity,convertBitmapToByte,bitmap);


    jsize num_bytes = env->GetArrayLength(byteArr);

    jbyte* elements = env->GetByteArrayElements(byteArr,nullptr);

    for(int i = 0;i< num_bytes;i++){
        contents.data.push_back(elements[i]);
    }

    return contents;




}

