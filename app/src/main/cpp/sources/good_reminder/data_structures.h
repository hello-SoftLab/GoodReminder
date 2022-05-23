#pragma once
#include "../global.h"

namespace structures {

    struct AppEntry {
        std::string text = "";
        std::string entryName = "";


    };
};
class AppObject;

template<typename T>
class AppComponent : public ecspp::ComponentSpecifier<T,AppObject> {


};

class AppObjectProperties {
    int hello = 0;
};

class AppObject : public ecspp::TaggedObject<AppObject,AppComponent<ecspp::ComponentHelpers::Null>,AppObjectProperties> {
public:
    AppObject(entt::entity e);


};


