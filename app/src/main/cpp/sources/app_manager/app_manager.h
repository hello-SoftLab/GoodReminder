#pragma once
#include "../android_data/android_data.h"
#include "SDL.h"
#include "../helpers/color.h"

class AppManager {
public:

    static ecspp::HelperClasses::FunctionSink<void()> Drawing();
    static void SetClearColor(Color color);
    static float DeltaTime();



private:

    static inline ecspp::HelperClasses::EventLauncher<void()> m_DrawingFunctions;
    static inline Color m_ClearColor = Color::White;
    static inline float m_DeltaTime = 0;
    static inline ImVec2 m_MouseDelta;


    static bool HandleFrameUpdate();
    static void BeginFrame();
    static void Draw();
    static void EndFrame();



    friend class AndroidData;
};

