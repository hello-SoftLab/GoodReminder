#pragma once
#include "../android_data/android_data.h"
#include "SDL.h"
#include "../helpers/helpers.h"

class AppManager {
public:

    static HelperClasses::FunctionSink<void()> Drawing();

private:

    static inline HelperClasses::EventLauncher<void()> m_DrawingFunctions;

    static bool HandleFrameUpdate();
    static void BeginFrame();
    static void Draw();
    static void EndFrame();


    friend class AndroidData;
};

//useful imgui functions

#include "imgui_internal.h"
namespace ImGui {
    void ScrollWhenDraggingOnVoid(const ImVec2 &delta, ImGuiMouseButton mouse_button);

}