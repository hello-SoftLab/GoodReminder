#include "current_day_stage.h"
#include "../android_data/android_data.h"
#include "calendar_stage.h"
#include "older_day_stage.h"
#include "thank_you_stage.h"

void CurrentDayStage::Init() {
    m_Data = "Click here to write!";
    m_ArrowTex = AndroidData::GetLoadedTexture("images/seta.png");
    AndroidData::onKeyboard().Connect(this,[](yael::event_receiver* rec,SDL_Event* event){
        if(event->key.keysym.sym == SDLK_AC_BACK){
            InitialWindow::SetProgramStage<CalendarStage>();
        }
    });
}

void CurrentDayStage::Update(float deltaTime) {

    ImGui::PushStyleColor(ImGuiCol_Button,Color(0,0,0,0).AsImVec4());
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,Color(0,0,0,0).AsImVec4());
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,Color(0,0,0,0).AsImVec4());
    if(ImGui::ImageButton((ImTextureID)m_ArrowTex.GetID(),ImVec2(AndroidData::GetMonitorSize().x/10,AndroidData::GetMonitorSize().y/30))){
        InitialWindow::SetProgramStage<CalendarStage>();
    }
    ImGui::PopStyleColor(3);


    std::string myStr = "Remind me of:";

    ImGui::SetCursorPosX(AndroidData::GetMonitorSize().x/2 - ImGui::CalcTextSize(myStr.c_str()).x/2);

    ImGui::Text("%s",myStr.c_str());

    ImGui::InvisibleButton("###",ImVec2(ImGui::GetFrameHeight(),ImGui::GetFrameHeight()));

    ImGui::SetCursorPosX(InitialWindow::GetMiddleWidgetCursorOffsetX());


    ImGui::PushStyleColor(ImGuiCol_FrameBg,InitialWindow::GetBgColor().AsImVec4());
    ImGui::InputTextMultiline("##TextForCurrentDay",&m_Data,ImVec2(InitialWindow::GetMiddleWidgetSizeX(),0),ImGuiInputTextFlags_CallbackAlways,[](ImGuiInputTextCallbackData* data){
        if(data->BufTextLen != 0) {
            if(data->EventKey != ImGuiKey_Backspace) {

                std::string myStr(data->Buf);

                if(data->EventChar != 0){
                    myStr.erase(myStr.begin() + data->CursorPos - 1);
                }

                auto &vec = InitialWindow::GetCurrentProgramStage().GetAs<CurrentDayStage>()->GetNewlineLocations();

                auto it = vec.rbegin();

                while (it != vec.rend()) {
                    myStr.erase(myStr.begin() + *it);
                    it++;
                }

                vec.clear();

                bool done = false;
                int current = 0;

                int offset = 0;
                auto offsetFunc = [&](int newlineLocation){
                    if(newlineLocation < data->CursorPos + offset){
                        offset++;
                    }
                };


                while(!done){

                    if(auto pos = myStr.find('\n',current);pos != std::string::npos) {
                        if (ImGui::CalcTextSize(myStr.substr(current,pos-current).c_str()).x < InitialWindow::GetMiddleWidgetSizeX()){
                            current = pos + 1;
                            continue;
                        }
                        else {
                            int position = current + 1;
                            std::string substring = myStr.substr(current,position-current);
                            while(ImGui::CalcTextSize(substring.c_str()).x < InitialWindow::GetMiddleWidgetSizeX()){
                                position += 1;
                                substring = myStr.substr(current,position-current);
                            }
                            myStr.insert(position-1,"\n");
                            current = position;
                            vec.push_back(position-1);
                            offsetFunc(position-1);
                            continue;
                        }
                    }
                    else {
                        if (ImGui::CalcTextSize(myStr.substr(current,pos-current).c_str()).x < InitialWindow::GetMiddleWidgetSizeX()){
                            done = true;
                        }
                        else {
                            int position = current + 1;
                            std::string substring = myStr.substr(current,position-current);
                            while(ImGui::CalcTextSize(substring.c_str()).x < InitialWindow::GetMiddleWidgetSizeX()){
                                position += 1;
                                substring = myStr.substr(current,position-current);
                            }
                            myStr.insert(position-1,"\n");
                            current = position;
                            vec.push_back(position-1);
                            offsetFunc(position-1);
                            continue;
                        }
                    }

                }

                if(data->EventChar != 0) {
                    myStr.insert(myStr.begin() + data->CursorPos + offset, data->EventChar);
                }

                data->DeleteChars(0,data->BufTextLen);



                data->InsertChars(0,myStr.c_str());


            }



        }

        return 0;
    },nullptr);
    ImGui::BeginChild("##TextForCurrentDay");
    HandleScroll();
    ImGui::EndChild();
    ImGui::PopStyleColor();

    if(m_ShouldSetFocusOnText){
        ImGui::SetKeyboardFocusHere(-1);
        m_ShouldSetFocusOnText = false;
    }

    if(ImGui::IsItemActive() && m_ShouldClearBuffer){
        m_Data = "";
        ImGui::ClearActiveID();
        m_ShouldClearBuffer = false;
        m_ShouldSetFocusOnText = true;
    }




    ImGui::SetCursorPosX(AndroidData::GetMonitorSize().x/2 - ImGui::CalcTextSize("Remind me").x/2);

    if(ImGui::Button("Remind me")){

        auto it = m_NewlineLocations.rbegin();
        while(it != m_NewlineLocations.rend()){
            m_Data.erase(m_Data.begin() + *it);
            it++;
        }

        InitialWindow::SaveStringToDate(m_Day,m_Month,m_Year,m_Data);



        InitialWindow::SetProgramStage<ThankYouStage>();
    }

#ifndef NDEBUG
    ImGui::SetCursorPosX(AndroidData::GetMonitorSize().x/2 - ImGui::CalcTextSize("Test Day Output").x/2);

    if(ImGui::Button("Test Day Output")){
        InitialWindow::SetProgramStage<OlderDayStage>().onInit().Connect([&]() {
            InitialWindow::GetCurrentProgramStage().GetAs<OlderDayStage>()->SetDate(m_Year, m_Month,
                                                                                    m_Day);
        });
    }
#endif

}

std::vector<int> &CurrentDayStage::GetNewlineLocations() {
    return m_NewlineLocations;
}

void CurrentDayStage::SetDate(int year, int month, int day) {

    m_Day = day;
    m_Month = month;
    m_Year = year;
}
