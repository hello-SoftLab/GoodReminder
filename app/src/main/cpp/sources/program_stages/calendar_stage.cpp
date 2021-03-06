#include "calendar_stage.h"
#include "../../vendor/datetime/include/asap/asap.h"
#include "../../vendor/datetime/include/asap/duration.h"
#include "../android_data/android_data.h"
#include "../../vendor/datetime/include/asap/literals.h"
#include "../app_manager/app_manager.h"
#include "older_day_stage.h"
#include "current_day_stage.h"

void CalendarStage::Init() {

    m_LowerBound = asap::now().year();
    m_UpperBound = m_LowerBound + 1;

    m_ArrowTex = LoadTextureFromFile<Type2D>(AndroidData::GetDataDir() + "/assets/images/seta.png");




}


void CalendarStage::Update(float deltaTime) {

    using namespace asap::literals;


    ImGui::SetCursorPosX(InitialWindow::GetMiddleWidgetCursorOffsetX());
    ImGui::SetCursorPosY(AndroidData::GetMonitorSize().y/15);

    if(!hasInit){
        ImGui::SetWindowFontScale(static_cast<int>(3.0f/100.0f * AndroidData::GetMonitorSize().y/ImGui::GetDefaultFont()->FontSize));
        hasInit = true;
    }

    ImGui::PushStyleColor(ImGuiCol_ChildBg,InitialWindow::GetBgColor().AsImVec4());
    ImGui::PushStyleColor(ImGuiCol_Border,Color::White.AsImVec4());
    ImGui::BeginChild("CalendarChild",ImVec2(InitialWindow::GetMiddleWidgetSizeX(),12*AndroidData::GetMonitorSize().y/15),true,ImGuiWindowFlags_NoScrollbar);
    ImGui::PopStyleColor(2);

    auto endDate = asap::datetime(m_LowerBound,0,1) + 1_year - 1_day;

    if(endDate.year() == asap::now().year()){
        endDate = asap::now();
    }

    for(auto month : (asap::datetime(m_LowerBound,0,1)).until(endDate).every(1_month)){

        if(month.month() == 0){
            month += 1_day;
            month -= 1_day;
        }

        ImGui::SetCursorPosX(ImGui::GetWindowSize().x/2 - ImGui::CalcTextSize(GetMonthName(month.month()).c_str()).x/2);
        ImGui::Text("%s", GetMonthName(month.month()).c_str());


        if(ImGui::BeginTable(("##TableForMonth" + std::to_string(month.month()) + "OfYear" + std::to_string(m_CurrentYear)).c_str(),7)) {

            auto endDay = month + 1_month - 1_day;

            if(month.year() == asap::now().year() && month.month() == asap::now().month()){
                endDay = asap::now();
            }


            for (auto day: month.until(endDay).every(1_day)) {

                while(day.wday() != ImGui::TableGetColumnIndex()){
                    ImGui::TableNextColumn();
                }

                SetupInnerSquare(m_LowerBound,month.month(),day.mday());

            }
            ImGui::EndTable();
        }
    }

    HandleScroll();


    ImGui::EndChild();


    float xPos = AndroidData::GetMonitorSize().x/2 - ImGui::CalcTextSize(std::to_string(m_LowerBound).c_str()).x/2;


    ImGui::SetCursorPosX(xPos - ImGui::GetFrameHeight()/2 - ImGui::GetFrameHeight());
    ImGui::PushStyleColor(ImGuiCol_Button,Color(0,0,0,0).AsImVec4());
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,Color(0,0,0,0).AsImVec4());
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,Color(0,0,0,0).AsImVec4());
    ImGui::PushID(ImGui::GetID("LastYearButton"));
    if(ImGui::ImageButton((ImTextureID)m_ArrowTex.GetID(),ImVec2(ImGui::GetFrameHeight(),ImGui::GetFrameHeight()))){
        m_LowerBound--;
    }
    ImGui::PopID();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();

    ImGui::SetCursorPosX(xPos);

    ImGui::Text("%s", std::to_string(m_LowerBound).c_str());

    if(m_LowerBound != asap::now().year()) {

        ImGui::SameLine();

        ImGui::SetCursorPosX(xPos + ImGui::GetFrameHeight() + ImGui::GetFrameHeight());
        ImGui::PushStyleColor(ImGuiCol_Button,Color(0,0,0,0).AsImVec4());
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,Color(0,0,0,0).AsImVec4());
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,Color(0,0,0,0).AsImVec4());
        if(ImGui::ImageButton((ImTextureID)m_ArrowTex.GetID(),ImVec2(ImGui::GetFrameHeight(),ImGui::GetFrameHeight()),ImVec2(1,0),ImVec2(0,1))){
            m_LowerBound++;
        }
        ImGui::PopStyleColor(3);


    }
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
}

void CalendarStage::SetupInnerSquare(int year,int month,int day) {
    ImVec2 cursorPos = ImGui::GetCursorPos();

    ImVec2 size = ImVec2(ImGui::CalcTextSize("AA").x*1.5,ImGui::CalcTextSize("AA").y*1.5);
    if(day == asap::now().mday() && month == asap::now().month() && year == asap::now().year()){
        ImGui::PushStyleColor(ImGuiCol_Button,Color(240,142,101).AsImVec4());
        if(ImGui::Button(("##buttonForSelectionMonth" + std::to_string(month) + "Day" + std::to_string(day)).c_str(),size)){
            InitialWindow::SetProgramStage<CurrentDayStage>().onInit().Connect([=](){
                InitialWindow::GetCurrentProgramStage().GetAs<CurrentDayStage>()->SetDate(year,month,day);
            });
        }
        ImGui::PopStyleColor();
    }
    else {
        if(ImGui::Button(("##buttonForSelectionMonth" + std::to_string(month) + "Day" + std::to_string(day)).c_str(),size)){
            InitialWindow::SetProgramStage<OlderDayStage>().onInit().Connect([=](){
                InitialWindow::GetCurrentProgramStage().GetAs<OlderDayStage>()->SetDate(year,month,day);
            });
        }
    }


    ImGui::SetCursorPosX(cursorPos.x + size.x/2 - ImGui::CalcTextSize(std::to_string(day).c_str()).x/2);
    ImGui::SetCursorPosY(cursorPos.y + size.y/2 - ImGui::CalcTextSize(std::to_string(day).c_str()).y/2);
    ImGui::PushStyleColor(ImGuiCol_Text,Color::White.AsImVec4());
    ImGui::Text("%s",std::to_string(day).c_str());
    ImGui::PopStyleColor();

}

