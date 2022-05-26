#pragma once
#include "../good_reminder/initial_window.h"
#include "../opengl_wrappers/texture.h"

class CalendarStage : public ProgramStage {
public:
    void Init() override;
    void Update(float deltaTime) override;
    ~CalendarStage();

private:
    std::string GetMonthName(int month);
    void SetupInnerSquare(int year,int month,int day);


    int m_CurrentYear;
    int m_LowerBound,m_UpperBound;
    bool m_ShouldScroll = false;
    float m_ScrollValue = 0;
    float m_ScrollFriction = 40;
    float m_ScrollAccel;
    size_t m_ConnectionID = 0;


};