#include "older_day_stage.h"
#include "../android_data/android_data.h"
#include "../global.h"
#include "calendar_stage.h"

void OlderDayStage::Init() {

    auto vec = InitialWindow::GetSavedStringsByDate(m_Day,m_Month,m_Year);

    if(!vec.empty()){
        m_Data = vec;
    }

}

void OlderDayStage::Update(float deltaTime) {

    if(ImGui::ArrowButton("##ReturnArrow",ImGuiDir_Left)){
        InitialWindow::SetProgramStage<CalendarStage>();
    }
    if(m_Data.empty()){
        std::string myStr =
                "I'm sorry, no text was written this day...";

        ImGui::SetCursorPosX(
                AndroidData::GetMonitorSize().x / 2 - ImGui::CalcTextSize(myStr.c_str()).x / 2);

        ImGui::Text("%s", myStr.c_str());
    }
    else {

        std::string myStr =
                "On " + std::to_string(m_Day) + "." + std::to_string(m_Month + 1) + "." +
                std::to_string(m_Year) + "...";

        ImGui::SetCursorPosX(
                AndroidData::GetMonitorSize().x / 2 - ImGui::CalcTextSize(myStr.c_str()).x / 2);

        ImGui::Text("%s", myStr.c_str());


        for (auto &text: m_Data) {
            ImGui::SetCursorPosX(AndroidData::GetMonitorSize().x / 7);
            ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize,2);
            ImGui::PushStyleColor(ImGuiCol_Border,Color::White.AsImVec4());
            ImGui::BeginChild(("##olderDayChild" + ecspp::HelperFunctions::GenerateStringHash(&text)).c_str(),ImVec2(5*AndroidData::GetMonitorSize().x/7,(1.5f + (int)ImGui::CalcTextSize(text.c_str()).x/(int)(5*AndroidData::GetMonitorSize().x/7)) * ImGui::CalcTextSize("A").y),true,ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::PopStyleVar();
            ImGui::PopStyleColor();


            ImGui::TextWrapped("%s", text.c_str());

            ImGui::EndChild();


        }


        HandleScroll();


    }






}

void OlderDayStage::SetDate(int year, int month, int day) {
    m_Month = month;
    m_Year = year;
    m_Day = day;
}
