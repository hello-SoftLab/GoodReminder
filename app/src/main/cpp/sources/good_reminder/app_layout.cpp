#include "app_layout.h"
#include "../app_manager/app_manager.h"
void AppLayout::Init() {

    AppManager::Drawing().Connect([](){
        ImGui::SetWindowFontScale(2);


        ImGui::Text("Hey!");







    });


}
