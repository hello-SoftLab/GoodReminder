#pragma once
#include "../good_reminder/initial_window.h"
#include "../opengl_wrappers/texture.h"
#include "scrollable.h"

class CalendarStage : public ProgramStage, public Scrollable {
public:
    void Init() override;
    void Update(float deltaTime) override;
    ~CalendarStage();

private:
    std::string GetMonthName(int month);
    void SetupInnerSquare(int year,int month,int day);


    int m_CurrentYear;
    int m_LowerBound,m_UpperBound;



};