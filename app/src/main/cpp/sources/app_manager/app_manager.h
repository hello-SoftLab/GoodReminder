#pragma once
#include "../android_data/android_data.h"
#include "SDL.h"
#include "../helpers/color.h"

class AppManager {
public:

    static yael::event_sink<void()> Drawing();
    static void SetClearColor(Color color);
    static float DeltaTime();



private:

    static inline yael::event_launcher<void()> m_DrawingFunctions;
    static inline Color m_ClearColor = Color::White;
    static inline float m_DeltaTime = 0;
    static inline ImVec2 m_MouseDelta;


    static bool HandleFrameUpdate();
    static void BeginFrame();
    static void Draw();
    static void EndFrame();



    friend class AndroidData;
};

