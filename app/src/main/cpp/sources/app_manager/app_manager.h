#pragma once
#include "../android_data/android_data.h"

class AppManager {

private:
    static void HandleFrameUpdate();
    static void BeginFrame();
    static void Draw();
    static void EndFrame();


    friend class AndroidData;
};
