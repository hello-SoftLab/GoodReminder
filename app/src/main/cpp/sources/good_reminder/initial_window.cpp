#include "initial_window.h"
#include "../global.h"
#include "../android_data/android_data.h"
#include "../app_manager/app_manager.h"
#include "../program_stages/loading_stage.h"
#include <fstream>

void InitialWindow::Update() {

    if(m_Properties.m_CurrentStage){
        m_Properties.m_CurrentStage.Get()->Update(AppManager::DeltaTime());
    }



    m_NextProgramStage.m_UpdatePropertiesFunc();
    m_NextProgramStage.m_UpdatePropertiesFunc = [](){};

}

void InitialWindow::ClearProgramStage() {
  m_NextProgramStage.m_UpdatePropertiesFunc = [&](){
      m_Properties.m_CurrentStage.ClearCurrentType();
  };
  m_NextProgramStage.onInitEvent = yael::event_launcher<void()>();

};

void InitialWindow::Init() {


    AppManager::SetClearColor(m_BgColor); //
    InitialWindow::SetProgramStage<LoadingStage>();

    std::string dir = AndroidData::GetDataDir() + "/data_per_year.yaml";



    if(std::filesystem::exists(dir)){
        m_MainNode = YAML::LoadFile(dir);
    }




    AndroidData::onPause().Connect(&SaveToFile);

    /*
    auto it = m_UnloadedFonts.begin();
    while(it != m_UnloadedFonts.end()){
        auto& [name,data] = *it;
        ImFontConfig cfg;
        cfg.FontDataOwnedByAtlas = false;
        ImFont* font = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(data.data(),data.size(),20.0f,&cfg);
        
        if(!font->IsLoaded()){
            continue;
        }
        m_LoadedFonts[name] = font;
        m_UnloadedFonts.erase(it);
        it = m_UnloadedFonts.begin();
    }
    */


    //ImGui::PushFont(m_LoadedFonts["LibreCaslonText-Regular"]);


}

yael::event_sink<void()> InitialWindow::OnAnimationFinish() {
    return {m_OnAnimationFinish};
}

ecspp::HelperClasses::PointerHolder<ProgramStage> InitialWindow::GetCurrentProgramStage() {
    auto watcher = ecspp::HelperClasses::PointerHolder<ProgramStage>();
    watcher.WatchPointer(m_Properties.m_CurrentStage);

    return watcher;
}

Color InitialWindow::GetBgColor() {
    return m_BgColor;
}

void InitialWindow::SaveStringToDate(int day, int month, int year, std::string data) {
    m_MainNode[year][month][day].push_back(data);

    SaveToFile();
}

std::vector<std::string> InitialWindow::GetSavedStringsByDate(int day, int month, int year) {
    if(!m_MainNode[year]){
        return {};
    }
    if(!m_MainNode[year][month]){
        return {};
    }
    if(!m_MainNode[year][month][day]){
        return {};
    }

    std::vector<std::string> data;

    for(auto node : m_MainNode[year][month][day]){
        if(node){
            data.push_back(node.as<std::string>());
        }
    }
    return data;
}

void InitialWindow::SaveToFile() {
    std::string dir = AndroidData::GetDataDir() + "/data_per_year.yaml";

    if(!std::filesystem::exists(AndroidData::GetDataDir())){
        std::filesystem::create_directory(AndroidData::GetDataDir());
    }

    std::ofstream stream;

    stream.open(dir);

    if(stream.is_open()){
        stream << InitialWindow::m_MainNode;
    }

    stream.close();
}

float InitialWindow::GetMiddleWidgetSizeX() {
    return 5*AndroidData::GetMonitorSize().x/7;
}

float InitialWindow::GetMiddleWidgetCursorOffsetX() {
    return AndroidData::GetMonitorSize().x/7;
}

ImFont *InitialWindow::GetFont(std::string name) {
    if(m_LoadedFonts.find(name) != m_LoadedFonts.end()){
        return m_LoadedFonts[name];
    }
    return nullptr;
}

void InitialWindow::LoadFont(std::string name, std::vector<unsigned char> data) {

    std::filesystem::path path = name;
    name = path.stem().string();

    m_UnloadedFonts[name] = data;
}


