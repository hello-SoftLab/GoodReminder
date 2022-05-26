#include "older_day_stage.h"
#include "../android_data/android_data.h"
#include "../global.h"
#include "calendar_stage.h"

void OlderDayStage::Init() {



}

void OlderDayStage::Update(float deltaTime) {

    if(ImGui::ArrowButton("##ReturnArrow",ImGuiDir_Left)){
        InitialWindow::SetProgramStage<CalendarStage>();
    }

    std::string myStr = "On " + std::to_string(m_Day) + "." + std::to_string(m_Month + 1) + "." + std::to_string(m_Year) + "...";

    ImGui::SetCursorPosX(AndroidData::GetMonitorSize().x/2 - ImGui::CalcTextSize(myStr.c_str()).x/2);

    ImGui::Text("%s",myStr.c_str());








}

void OlderDayStage::SetDate(int year, int month, int day) {
    m_Month = month;
    m_Year = year;
    m_Day = day;
}
