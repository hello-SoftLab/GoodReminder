#include "initial_window.h"
#include "../global.h"
#include "../android_data/android_data.h"
#include "../app_manager/app_manager.h"
#include "../program_stages/loading_stage.h"

void InitialWindow::Update() {

    if(m_Properties.m_CurrentStage){
        m_Properties.m_CurrentStage.Get()->Update(AppManager::DeltaTime());
    }



    m_UpdatePropertiesFunc();
    m_UpdatePropertiesFunc = [](){};

}

void InitialWindow::ClearProgramStage() {
  m_UpdatePropertiesFunc = [&](){
      m_Properties.m_CurrentStage.ClearCurrentType();
  };

};

void InitialWindow::Init() {


    AppManager::SetClearColor(Color(255,221,166)); //
    InitialWindow::SetProgramStage<LoadingStage>();
}

ecspp::HelperClasses::FunctionSink<void()> InitialWindow::OnAnimationFinish() {
    return {m_OnAnimationFinish};
}
