#pragma once
#include "../good_reminder/initial_window.h"

class CurrentDayStage : public ProgramStage {
public:
    void Init() override;
    void Update(float deltaTime) override;

    std::vector<int>& GetNewlineLocations();

private:
    std::string m_Data;
    std::vector<int> m_NewlineLocations;

};