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
    yael::event_sink<void()> onInit() {
        return {onInitEvent};
    }



private:
    std::function<void()> m_UpdatePropertiesFunc;
    yael::event_launcher<void()> onInitEvent;

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
    static yael::event_sink<void()> OnAnimationFinish();

    static void SaveStringToDate(int day,int month,int year,std::string data);
    static std::vector<std::string> GetSavedStringsByDate(int day,int month,int year);

    static float GetMiddleWidgetSizeX();
    static float GetMiddleWidgetCursorOffsetX();
    static ImFont* GetFont(std::string name);
    static void LoadFont(std::string name,std::vector<unsigned char> data);

private:
    static void SaveToFile();


    static inline std::unordered_map<std::string,ImFont*> m_LoadedFonts;
    static inline std::unordered_map<std::string,std::vector<unsigned char>> m_UnloadedFonts;
    static inline YAML::Node m_MainNode;
    static inline Color m_BgColor = Color(255,221,166);
    static inline ProgramStageEvents m_NextProgramStage;
    static inline yael::event_launcher<void()> m_OnAnimationFinish;
    static inline InitialWindowProperties m_Properties;

};