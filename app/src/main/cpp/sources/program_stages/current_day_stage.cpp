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
    ImGui::InputTextMultiline("##TextForCurrentDay",&m_Data,ImVec2(5*AndroidData::GetMonitorSize().x/7,0),ImGuiInputTextFlags_CallbackAlways,[](ImGuiInputTextCallbackData* data){
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
                        if (ImGui::CalcTextSize(myStr.substr(current,pos-current).c_str()).x < 5*AndroidData::GetMonitorSize().x/7){
                            current = pos + 1;
                            continue;
                        }
                        else {
                            int position = current + 1;
                            std::string substring = myStr.substr(current,position-current);
                            while(ImGui::CalcTextSize(substring.c_str()).x < 5*AndroidData::GetMonitorSize().x/7){
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
                        if (ImGui::CalcTextSize(myStr.substr(current,pos-current).c_str()).x < 5*AndroidData::GetMonitorSize().x/7){
                            done = true;
                        }
                        else {
                            int position = current + 1;
                            std::string substring = myStr.substr(current,position-current);
                            while(ImGui::CalcTextSize(substring.c_str()).x < 5*AndroidData::GetMonitorSize().x/7){
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
    ImGui::PopStyleColor();




    ImGui::SetCursorPosX(AndroidData::GetMonitorSize().x/2 - ImGui::CalcTextSize("Remind me").x/2);
    ImGui::Button("Remind me");


}

std::vector<int> &CurrentDayStage::GetNewlineLocations() {
    return m_NewlineLocations;
}
