#pragma once
#include "../global.h"

enum AnimationState {
    NoAnimation,
    InitialAnimation
};

struct InitialWindowProperties {
    int animationState = AnimationState::InitialAnimation;
    float animationTime = 0;
};

class InitialWindow {
public:
    static void Update();
    static void Init();
    static ecspp::HelperClasses::FunctionSink<void()> OnAnimationFinish();


private:
    static inline ecspp::HelperClasses::EventLauncher<void()> m_OnAnimationFinish;
    static inline InitialWindowProperties m_Properties;

};