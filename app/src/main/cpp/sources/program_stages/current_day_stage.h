#pragma once
#include "../good_reminder/initial_window.h"
#include "scrollable.h"

class CurrentDayStage : public ProgramStage, public Scrollable {
public:
    void Init() override;
    void Update(float deltaTime) override;

    std::vector<int>& GetNewlineLocations();
    void SetDate(int year,int month,int day);

private:
    bool m_ShouldSetFocusOnText = false;
    bool m_ShouldClearBuffer = true;
    int m_Year,m_Month,m_Day;
    std::string m_Data;
    std::vector<int> m_NewlineLocations;

};