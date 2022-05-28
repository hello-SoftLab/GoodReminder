#pragma once
#include "../good_reminder/initial_window.h"



class OlderDayStage : public ProgramStage {
public:
    void Init() override;
    void Update(float deltaTime) override;
    void SetDate(int year,int month,int day);

private:
    int m_Year,m_Month,m_Day;
    std::vector<std::string> m_Data;

};