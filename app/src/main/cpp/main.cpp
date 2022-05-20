#include "sources/global.h"
#include "sources/android_data/android_data.h"
#include "SDL.h"
#include "good_reminder/app_layout.h"


int SDL_main(int /*argc*/, char* /*argv*/[]) {

    AppLayout::Init();

    AndroidData::Init();
    return 0;
}



