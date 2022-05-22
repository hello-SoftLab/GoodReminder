#include "sources/global.h"
#include "sources/android_data/android_data.h"
#include "SDL.h"
#include "good_reminder/app_layout.h"

static bool firstLoop = true;

int SDL_main(int /*argc*/, char* /*argv*/[]) {

    if(firstLoop) {
        AppLayout::Init();
        firstLoop = false;
    }

    AndroidData::Init();
    return 0;
}



