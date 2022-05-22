#include "initial_window.h"
#include "../global.h"


void InitialWindow::Update() {






    if(m_Properties.normalAnimationCounter == -1){
        m_Properties.finalAnimationCounter++;
    } else {
        m_Properties.normalAnimationCounter++;

    }

}

void InitialWindow::Init() {
    
}

HelperClasses::FunctionSink<void()> InitialWindow::OnAnimationFinish() {
    return {m_OnAnimationFinish};
}
