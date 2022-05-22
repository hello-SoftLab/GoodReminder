#pragma once
#include "../vendor/imgui/imgui.h"
#include "../vendor/imgui/imgui_internal.h"
#include "../vendor/imgui/backends/imgui_impl_sdl.h"
#include "../vendor/imgui/backends/imgui_impl_opengl3.h"
#include "../vendor/imgui/misc/cpp/imgui_stdlib.h"
#include "../vendor/entt/single_include/entt/entt.hpp"
#include "android/log.h"
#include "fmt/format.h"
#include <jni.h>
#include <iostream>
#include "jni.h"
#include <string>
#include "GLES3/gl3.h"
#include "EGL/egl.h"
#include "android/asset_manager.h"
#include "../vendor/glm/glm/glm.hpp"
#include "SDL.h"
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


