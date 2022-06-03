#pragma once
#include "../global.h"

class AppLayout {
public:
    static void Init();
    static yael::event_sink<void()> onCleanup();

private:
    static inline yael::event_launcher<void()> m_CleanupEvent;

};