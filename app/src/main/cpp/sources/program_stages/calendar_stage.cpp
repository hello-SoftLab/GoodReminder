#include "calendar_stage.h"
#include "../../vendor/datetime/include/asap/asap.h"
#include "../../vendor/datetime/include/asap/duration.h"
#include "../android_data/android_data.h"
#include "../../vendor/datetime/include/asap/literals.h"
#include "../app_manager/app_manager.h"

void CalendarStage::Init() {

    m_LowerBound = asap::now().year();
    m_UpperBound = m_LowerBound + 1;

    m_ConnectionID = AndroidData::onFingerEvent().Connect([&](SDL_Event* event){
        if(event->tfinger.type == SDL_FINGERMOTION){
            m_ScrollDelta.x = event->tfinger.dx * AndroidData::GetMonitorSize().x;
            m_ScrollDelta.y = event->tfinger.dy * AndroidData::GetMonitorSize().y;
            m_ShouldScroll = true;
        }
    });
}


void CalendarStage::Update(float deltaTime) {

    using namespace asap::literals;
    ImGui::SetCursorPosX(AndroidData::GetMonitorSize().x/7);
    ImGui::SetCursorPosY(AndroidData::GetMonitorSize().y/4);

    ImGui::BeginChild("CalendarChild",ImVec2(5*AndroidData::GetMonitorSize().x/7,3*AndroidData::GetMonitorSize().y/5),ImGuiWindowFlags_NoBackground);
    ImGui::SetWindowFontScale(3);
    for(auto month : asap::datetime(m_LowerBound,0,0).until(asap::datetime(m_UpperBound,0,0)).every(1_month)){

        if(month.month() == 0){
            ImGui::Text("%s",std::to_string(month.year()).c_str());
        }

        ImGui::Text("%s", GetMonthName(month.month()).c_str());
        if(month.month() == 0 && m_LowerBound == month.year()){
            if(ImGui::IsItemVisible()){
                //m_LowerBound--;
            }
        }
        if(month.month() == 5){
            if(ImGui::IsItemVisible()){
                m_LowerBound = month.year();
                m_UpperBound = m_LowerBound + 1;
            }
        }
        if(month.month() == 11 && month.year() == m_UpperBound){
            if(ImGui::IsItemVisible()){
                //m_UpperBound++;
            }
        }

        if(ImGui::BeginTable(("##TableForMonth" + std::to_string(month.month()) + "OfYear" + std::to_string(m_CurrentYear)).c_str(),7)) {
            for (auto day: month.until(month + 1_month).every(1_day)) {
                ImGui::TableNextColumn();
                if(ImGui::Selectable(std::to_string(day.mday()).c_str(),false,ImGuiSelectableFlags_SpanAvailWidth)){

                }


            }
            ImGui::EndTable();
        }
    }

    if(m_ShouldScroll){
        ImGui::SetScrollY(ImGui::GetScrollY() - m_ScrollDelta.y);
        m_ShouldScroll = false;
    }


    ImGui::EndChild();





}

std::string CalendarStage::GetMonthName(int month) {
    switch(month){
        case 0:
            return "January";
        case 1:
            return "February";
        case 2:
            return "March";
        case 3:
            return "April";
        case 4:
            return "May";
        case 5:
            return "June";
        case 6:
            return "July";
        case 7:
            return "August";
        case 8:
            return "September";
        case 9:
            return "October";
        case 10:
            return "November";
        case 11:
            return "December";
    }
}

CalendarStage::~CalendarStage() {
    AndroidData::onFingerEvent().Disconnect(m_ConnectionID);
}

