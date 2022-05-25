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


    int m_CurrentYear;
    int m_LowerBound,m_UpperBound;
    bool m_ShouldScroll = false;
    ImVec2 m_ScrollDelta;
    size_t m_ConnectionID = 0;


};