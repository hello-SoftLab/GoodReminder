#pragma once
#include "../global.h"

class ProgramStage {
public:
    virtual void Init() {};
    virtual void Update(float deltaTime) {};
};

struct InitialWindowProperties {
    ecspp::HelperClasses::PointerHolder<ProgramStage> m_CurrentStage;
};

class InitialWindow {
public:

    static void ClearProgramStage();

    template<typename T>
    static void SetProgramStage() {
        m_UpdatePropertiesFunc = [&](){
            if(!m_Properties.m_CurrentStage.template IsHoldingType<T>()) {
                T &obj = m_Properties.m_CurrentStage.template HoldType<T>();
                obj.Init();
            }
        };

    };
    static void Update();
    static void Init();
    static ecspp::HelperClasses::FunctionSink<void()> OnAnimationFinish();


private:
    static inline std::function<void()> m_UpdatePropertiesFunc;
    static inline ecspp::HelperClasses::EventLauncher<void()> m_OnAnimationFinish;
    static inline InitialWindowProperties m_Properties;

};