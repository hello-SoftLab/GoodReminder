#pragma once
#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui_internal.h"
#include "../vendor/imgui/backends/imgui_impl_sdl.h"
#include "../vendor/imgui/backends/imgui_impl_opengl3.h"
#include "../vendor/imgui/misc/cpp/imgui_stdlib.h"
#include "../vendor/entt/single_include/entt/entt.hpp"
#include "android/log.h"
#include "android/obb.h"
#include "fmt/format.h"
#include "../vendor/datetime/include/asap/datetime.h"
#include "../vendor/yaml/include/yaml-cpp/yaml.h"
#include "../vendor/yael/include/yael.h"
#include <jni.h>
#include <iostream>
#include "jni.h"
#include <string>
#include "GLES3/gl3.h"
#include "EGL/egl.h"
#include "android/asset_manager.h"
#include "../vendor/glm/glm/glm.hpp"
#include "../vendor/glm/glm/gtc/type_ptr.hpp"
#include "SDL.h"
#include "ecspp.h"
#include "android_native_app_glue.h"
#include "android/asset_manager_jni.h"
#include <sstream>

using namespace std;


static void ClearGLErrors() {
    while (glGetError() != GL_NO_ERROR) {
        ;
    }
}

bool GetGLError(int line, std::string file);

#ifdef NDEBUG
#define GL_CALL(x) x
#define GL_CALL_WITH_RESULT(var,x) var = x
#else
#define GL_CALL(x) ClearGLErrors(); x; if(GetGLError(__LINE__,__FILE__)) {;}
#define GL_CALL_WITH_RESULT(var,x) ClearGLErrors(); var = x; if(GetGLError(__LINE__,__FILE__)) {;}
#endif


#define GL_SIZEOF(x) TestSize(x)


static unsigned int TestSize(unsigned int dataType) {
    switch (dataType) {
        case GL_FLOAT:
            return sizeof(float);
        case GL_UNSIGNED_INT:
            return sizeof(unsigned int);
        case GL_UNSIGNED_BYTE:
            return sizeof(unsigned char);
        case GL_BYTE:
            return sizeof(char);
        case GL_INT:
            return sizeof(int);
        default:
            return 0;
    }
}


#include "imgui_internal.h"
namespace ImGui {
    void ScrollWhenDraggingOnVoid(const ImVec2 &delta, ImGuiMouseButton mouse_button);

};
