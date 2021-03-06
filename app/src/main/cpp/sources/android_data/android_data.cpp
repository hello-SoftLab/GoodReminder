#include "android_data.h"
#include "../app_manager/app_manager.h"
#include "../good_reminder/app_layout.h"

void AndroidData::Init() {

    Initialization();

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

    for(auto& [name,contents] : m_ImagesToBeLoaded){
        LoadImage(name);
    }


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
    SDL_GetDisplayUsableBounds(0,&m_Dimensions);

    TimedFunctionCall(5,[&](){
        SDL_GetDisplayUsableBounds(0,&m_Dimensions);
    });


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

    onFingerEvent().Connect([](SDL_Event* ev){
        if(ev->tfinger.type == SDL_FINGERMOTION){
            m_DragDelta.x = ev->tfinger.dx * AndroidData::GetMonitorSize().x;
            m_DragDelta.y = ev->tfinger.dy * AndroidData::GetMonitorSize().y;
        }
    });

    static bool firstLoop = true;
    if(firstLoop) {
        AppLayout::Init();
        firstLoop = false;
    }

    return true;
}

SDL_Window *AndroidData::CurrentWindow() {
    return m_WindowPointer;
}

yael::event_sink<void()> AndroidData::onDestroy() {
    return {m_Terminating};
}

yael::event_sink<void()> AndroidData::onLowMemory() {
    return {m_LowMemoryEvent};
}

yael::event_sink<void()> AndroidData::onPause() {
    return {m_DidEnterBgEvent};
}

yael::event_sink<void()> AndroidData::onResume() {
    return {m_DidEnterFgEvent};
}

yael::event_sink<void(int,int)> AndroidData::onResize() {
    return {m_ResizeEvent};
}
yael::event_sink<void(SDL_Event*)> AndroidData::onFingerEvent() {
    return {m_FingerDownEvent};
}
yael::event_sink<void(SDL_MultiGestureEvent)> AndroidData::onMultiGesture() {
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
                m_ResizeEvent.EmitEvent((int)event->window.data1,(int)event->window.data2);
            }
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
    return {static_cast<float>(m_Dimensions.w),static_cast<float>(m_Dimensions.h)};
}

void AndroidData::LoadImage(std::string name) {

    if(m_ImagesToBeLoaded.find(name) == m_ImagesToBeLoaded.end()){
        return;
    }

    LoadedFileContents contents = m_ImagesToBeLoaded[name];

    Texture<Type2D> tex(contents.width,contents.height,[=](Texture<Type2D>& texture){

        texture.Bind();
        GL_CALL(glTexParameteri(texture.GetType(), GL_TEXTURE_WRAP_S, GL_REPEAT));
        GL_CALL(glTexParameteri(texture.GetType(), GL_TEXTURE_WRAP_T, GL_REPEAT));
        GL_CALL(glTexParameteri(texture.GetType(), GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_CALL(glTexParameteri(texture.GetType(), GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
        if (ecspp::HelperFunctions::HashClassName<Type2D>() == ecspp::HelperFunctions::HashClassName<Type2D>()) {
            GL_CALL(glTexImage2D(texture.GetType(), 0, GL_RGBA, contents.width, contents.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, contents.data.data()));
        }
        GL_CALL(glGenerateMipmap(texture.GetType()));

        texture.Unbind();
    });

    if(tex){
        m_LoadedImages[name] = tex;
    }
}

Texture<Type2D> AndroidData::GetLoadedTexture(std::string path) {
    if(m_LoadedImages.find(path) != m_LoadedImages.end()){
        return m_LoadedImages[path];
    }
    return {};
}

void AndroidData::SetImageToBeLoaded(std::string name, std::vector<unsigned char> data, int width,
                                     int height) {
    LoadedFileContents contents;
    contents.data = data;
    contents.width = width;
    contents.height = height;
    m_ImagesToBeLoaded[name] = contents;
}

ImVec2 AndroidData::GetDragDelta() {
    return m_DragDelta;
}

void AndroidData::TimedFunctionCall(float delay, std::function<void()> func) {
    DelayedFunctionStructure prop;
    prop.maxTime = delay;
    prop.m_Function = func;

    m_DelayedFunctions.push_back(prop);
}

void AndroidData::SetDataDir(std::string name) {
    m_DataDir = name;
}

std::string AndroidData::GetDataDir() {
    return m_DataDir;
}

void AndroidData::Initialization() {
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



    while(AppManager::HandleFrameUpdate() && m_IsRunning){

        SDL_Event event;
        while(SDL_PollEvent(&event)){
            ImGui_ImplSDL2_ProcessEvent(&event);
            switch(event.type){
                case SDL_QUIT:
                    m_IsRunning = false;
                    break;
                case SDL_WINDOWEVENT:
                    if(event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(AndroidData::CurrentWindow())){
                        m_IsRunning = false;
                        break;
                    }
                    break;
                case SDL_MULTIGESTURE:
                    m_MultiGestureEvent.EmitEvent(SDL_MultiGestureEvent(event.mgesture));
                    break;
                case SDL_FINGERDOWN:
                    m_FingerDownEvent.EmitEvent(&event);
                    break;
                case SDL_FINGERUP:
                    m_FingerDownEvent.EmitEvent(&event);
                    break;
                case SDL_FINGERMOTION:
                    m_FingerDownEvent.EmitEvent(&event);
                    break;
                case SDL_KEYDOWN:
                    m_KeyboardEvent.EmitEvent(&event);
                    break;
                case SDL_KEYUP:
                    m_KeyboardEvent.EmitEvent(&event);
                    break;


            }
        }

        m_DragDelta = ImVec2(0,0);

        auto it = m_DelayedFunctions.begin();
        while(it != m_DelayedFunctions.end()){

            if(it->currentTimer > it -> maxTime){
                it->m_Function();
                it = m_DelayedFunctions.erase(it);
                continue;
            }

            it->currentTimer += AppManager::DeltaTime();


            it++;
        }

        SDL_GetDisplayUsableBounds(0,&m_Dimensions);


    }

    Cleanup();
}

void AndroidData::Cleanup() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(m_Context);
    // Close and destroy the window
    SDL_DestroyWindow(m_WindowPointer);

    // Clean up
    SDL_Quit();
}

yael::event_sink<void(SDL_Event *)> AndroidData::onKeyboard() {
    return {m_KeyboardEvent};
}




