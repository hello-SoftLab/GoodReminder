#pragma once
#include "../global.h"
#include "../helpers/color.h"
#include "app_layout.h"

class ProgramStage {
public:
    virtual void Init() {};
    virtual void Update(float deltaTime) {};
};

struct ProgramStageEvents {
public:
    ecspp::HelperClasses::FunctionSink<void()> onInit() {
        return {onInitEvent};
    }



private:
    std::function<void()> m_UpdatePropertiesFunc;
    ecspp::HelperClasses::EventLauncher<void()> onInitEvent;

    friend class InitialWindow;

};

struct InitialWindowProperties {
    ecspp::HelperClasses::PointerHolder<ProgramStage> m_CurrentStage;
};

class InitialWindow {
public:

    static void ClearProgramStage();
    static Color GetBgColor();

    template<typename T>
    static ProgramStageEvents& SetProgramStage() {
        m_NextProgramStage.m_UpdatePropertiesFunc = [&](){
            if(!m_Properties.m_CurrentStage.template IsHoldingType<T>()) {
                T &obj = m_Properties.m_CurrentStage.template HoldType<T>();
                m_NextProgramStage.onInitEvent.EmitEvent();
                obj.Init();
            }
        };
        if(!m_Properties.m_CurrentStage.template IsHoldingType<T>()){
            m_NextProgramStage.onInitEvent.Clear();
        }
        return m_NextProgramStage;

    };

    static ecspp::HelperClasses::PointerHolder<ProgramStage> GetCurrentProgramStage();

    static void Update();
    static void Init();
    static ecspp::HelperClasses::FunctionSink<void()> OnAnimationFinish();

    static void SaveStringToDate(int day,int month,int year,std::string data);
    static std::vector<std::string> GetSavedStringsByDate(int day,int month,int year);


private:
    static inline YAML::Node m_MainNode;
    static inline Color m_BgColor = Color(255,221,166);
    static inline ProgramStageEvents m_NextProgramStage;
    static inline ecspp::HelperClasses::EventLauncher<void()> m_OnAnimationFinish;
    static inline InitialWindowProperties m_Properties;

};