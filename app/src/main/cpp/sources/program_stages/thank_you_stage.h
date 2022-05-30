#pragma once
#include "../good_reminder/initial_window.h"

class ThankYouStage : public ProgramStage {
public:

    ~ThankYouStage();
    void Init() override;
    void Update(float deltaTime) override;

private:
    float counter = 0;
    size_t connectionID;

};