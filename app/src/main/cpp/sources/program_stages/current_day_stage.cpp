#include "current_day_stage.h"
#include "../android_data/android_data.h"
#include "calendar_stage.h"

void CurrentDayStage::Init() {
    m_Data = "Click here to write!";

}

void CurrentDayStage::Update(float deltaTime) {


    if(ImGui::ArrowButton("##ReturnArrowFromCurrentDay",ImGuiDir_Left)){
        InitialWindow::SetProgramStage<CalendarStage>();
    }

    std::string myStr = "Remind me of:";

    ImGui::SetCursorPosX(AndroidData::GetMonitorSize().x/2 - ImGui::CalcTextSize(myStr.c_str()).x/2);

    ImGui::Text("%s",myStr.c_str());

    ImGui::InvisibleButton("###",ImVec2(ImGui::GetFrameHeight(),ImGui::GetFrameHeight()));

    ImGui::SetCursorPosX(AndroidData::GetMonitorSize().x/7);


    ImGui::PushStyleColor(ImGuiCol_FrameBg,InitialWindow::GetBgColor().AsImVec4());
    ImGui::InputTextMultiline("##TextForCurrentDay",&m_Data,ImVec2(5*AndroidData::GetMonitorSize().x/7,0));
    ImGui::PopStyleColor();


    ImGui::SetCursorPosX(AndroidData::GetMonitorSize().x/2 - ImGui::CalcTextSize("Remind me").x/2);
    ImGui::Button("Remind me");


}
