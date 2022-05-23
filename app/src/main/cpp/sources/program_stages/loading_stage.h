#pragma once
#include "good_reminder/initial_window.h"

class LoadingStage : public ProgramStage {
public:
    void Init() override;
    void Update(float deltaTime) override;

private:
    float animationTime = 0;

};