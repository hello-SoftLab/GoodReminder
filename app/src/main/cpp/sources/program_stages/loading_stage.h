#pragma once
#include "good_reminder/initial_window.h"
#include "../opengl_wrappers/texture.h"

class LoadingStage : public ProgramStage {
public:
    void Init() override;
    void Update(float deltaTime) override;

private:
    float animationTime = 0;
    Texture<Type2D> m_Texture;

};