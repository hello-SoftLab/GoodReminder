#pragma once
#include "../global.h"
#include "../helpers/helpers.h"


struct InitialWindowProperties {
    int normalAnimationCounter = 0;
    int finalAnimationCounter = 0;
};

class InitialWindow {
public:
    static void Update();
    static void Init();
    static HelperClasses::FunctionSink<void()> OnAnimationFinish();


private:
    static inline HelperClasses::EventLauncher<void()> m_OnAnimationFinish;
    static inline InitialWindowProperties m_Properties;

};