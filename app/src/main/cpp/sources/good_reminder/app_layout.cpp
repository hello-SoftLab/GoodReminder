#include "app_layout.h"
#include "../app_manager/app_manager.h"
#include "initial_window.h"
void AppLayout::Init() {



    InitialWindow::Init();
    AppManager::Drawing().Connect([](){
        InitialWindow::Update();



    });




}
