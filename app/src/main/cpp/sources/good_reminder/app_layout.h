#pragma once
#include "../global.h"

class AppLayout {
public:
    static void Init();
    static ecspp::HelperClasses::FunctionSink<void()> onCleanup();

private:
    static inline ecspp::HelperClasses::EventLauncher<void()> m_CleanupEvent;

};