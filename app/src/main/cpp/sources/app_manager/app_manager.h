#pragma once
#include "../android_data/android_data.h"
#include "SDL.h"
class AppManager {

private:

    static bool HandleFrameUpdate();
    static void BeginFrame();
    static void Draw();
    static void EndFrame();


    friend class AndroidData;
};
