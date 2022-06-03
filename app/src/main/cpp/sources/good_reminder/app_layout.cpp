#include "app_layout.h"
#include "../app_manager/app_manager.h"
#include "initial_window.h"
void AppLayout::Init() {



    InitialWindow::Init();
    AppManager::Drawing().Connect([](){
        InitialWindow::Update();
    });

    AndroidData::onDestroy().Connect([&](){
        m_CleanupEvent.EmitEvent();
    });


}

yael::event_sink<void()> AppLayout::onCleanup() {
    return {m_CleanupEvent};
}


