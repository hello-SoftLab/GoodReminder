#pragma once
#include "../global.h"
#include "color.h"



namespace HelperClasses {
    template<typename>
    class Meta;
};




class HelperFunctions {
public:
    static inline std::vector<std::string> SplitString(const std::string &str, const std::string &delimiter = " ", const int max_elements = 0) {
        std::vector<std::string> tokens;
        std::string::size_type start_index = 0;
        while (true) {
            std::string::size_type next_index = str.find(delimiter, start_index);
            if (next_index == std::string::npos) {
                if(str.substr(start_index) != ""){
                tokens.push_back(str.substr(start_index));
                }
                break;
            } else {
                if(str.substr(start_index, next_index - start_index) != ""){
                    tokens.push_back(str.substr(start_index, next_index - start_index));
                }
                start_index = next_index + delimiter.length();
            }
            if (max_elements > 0 && tokens.size() == max_elements - 1) {
                tokens.push_back(str.substr(start_index));
                break;
            }
        }

        return tokens;
    }

    static bool StringReplace(std::string& word,const std::string& oldVal,std::string newVal);
    
    static size_t HashPtr(void* ptr);

    static std::string GenerateStringHash(void* ptr);

    static bool EraseWordFromString(std::string& mainWord, const std::string& wordToLookFor);

    template<typename T>
    static std::string GetClassName() {
        std::string name = std::string(entt::type_id<T>().name());
        HelperFunctions::EraseWordFromString(name, "class ");
        HelperFunctions::EraseWordFromString(name, "struct ");
        if (auto loc = name.find_last_of(':'); loc != std::string::npos) {
            name = std::string(name.begin() + loc + 1, name.end());
        }
        return name;
    }

    template<typename T>
    static std::string GetClassDisplayName() {
        std::string name = std::string(entt::type_id<T>().name());
        HelperFunctions::EraseWordFromString(name, "Component");
        HelperFunctions::EraseWordFromString(name, "class ");
        HelperFunctions::EraseWordFromString(name, "struct ");
        if (auto loc = name.find_last_of(':'); loc != std::string::npos) {
            name = std::string(name.begin() + loc + 1, name.end());
        }
        return name;
    }

    

    template<typename T>
    static entt::id_type HashClassName() {
        return entt::hashed_string(GetClassName<T>().c_str());
    }

    template<typename... Args>
    static entt::meta_any CallMetaFunction(std::string handle, std::string funcName,Args&&... args) {
        auto resolved = entt::resolve(entt::hashed_string(handle.c_str()));

        if (resolved) {
            if (auto func = resolved.func(entt::hashed_string(funcName.c_str())); func) {
                return func.invoke({}, std::forward<Args>(args)...);
            }
            return {};
        }
        return {};
    }

    static bool IsMetaClass(std::string className);
    static bool IsMetaFunction(const std::string& className, std::string funcName);
    


    




private:

    

    template<typename T,auto Fun,typename... Args>
    static constexpr auto CallFunctionForObject(entt::entity e, Args&&... args) {

        T obj(e);

        return std::invoke(Fun,&obj, std::forward<decltype(args)>(args)...);

    }

    


    template<typename Base,typename T, auto Func, typename... Args>
    static constexpr auto CallFunctionForObjectWithVirtualBase(entt::entity e, Args&&... args) {

        T obj(e);

        [&obj](auto&&... ar) {
            (((Base*)(&std::forward<T>(obj)))->*Func)(std::forward<decltype(ar)>(ar)...);
        }(std::forward<decltype(args)>(args)...);


        return true;

    };

    template<typename>
    friend class HelperClasses::Meta;
    
    
};

namespace HelperClasses {
    class Null {
    private:
        int dummy = 0;
    };

    template<typename T>
    class Meta {
    public:

        template<auto Func>
        bool RegisterFunc(std::string funcMetaName) {
            entt::meta<T>().type(hash).template func<Func>(entt::hashed_string(funcMetaName.c_str()));
            return true;
        };

        template<typename Base,auto Func> 
        bool RegisterVirtualMemberFunc(std::string funcMetaName) {
            entt::meta<T>().type(hash).template func<&HelperFunctions::CallFunctionForObjectWithVirtualBase<Base,T, Func>>(entt::hashed_string(funcMetaName.c_str()));
            return true;
        }

        template<auto Func>
        bool RegisterMemberFunc(std::string funcMetaName) {
            entt::meta<T>().type(hash).template func<&HelperFunctions::CallFunctionForObject<T, Func>>(entt::hashed_string(funcMetaName.c_str()));
            return true;
        }
    private:
        entt::id_type hash = HelperFunctions::HashClassName<T>();

    };

}

