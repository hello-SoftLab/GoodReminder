#include "initial_window.h"
#include "../global.h"
#include "../android_data/android_data.h"
#include "../app_manager/app_manager.h"
#include "../program_stages/loading_stage.h"

void InitialWindow::Update() {

    if(m_Properties.m_CurrentStage){
        m_Properties.m_CurrentStage.Get()->Update(AppManager::DeltaTime());
    }



    m_NextProgramStage.m_UpdatePropertiesFunc();
    m_NextProgramStage.m_UpdatePropertiesFunc = [](){};

}

void InitialWindow::ClearProgramStage() {
  m_NextProgramStage.m_UpdatePropertiesFunc = [&](){
      m_Properties.m_CurrentStage.ClearCurrentType();
  };
  m_NextProgramStage.onInitEvent = ecspp::HelperClasses::EventLauncher<void()>();

};

void InitialWindow::Init() {


    AppManager::SetClearColor(m_BgColor); //
    InitialWindow::SetProgramStage<LoadingStage>();
}

ecspp::HelperClasses::FunctionSink<void()> InitialWindow::OnAnimationFinish() {
    return {m_OnAnimationFinish};
}

ecspp::HelperClasses::PointerHolder<ProgramStage> InitialWindow::GetCurrentProgramStage() {
    auto watcher = ecspp::HelperClasses::PointerHolder<ProgramStage>();
    watcher.WatchPointer(m_Properties.m_CurrentStage);

    return watcher;
}

Color InitialWindow::GetBgColor() {
    return m_BgColor;
}
