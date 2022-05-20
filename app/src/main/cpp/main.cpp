#include "sources/global.h"
#include "sources/android_data/android_data.h"
#include "SDL.h"



int SDL_main(int /*argc*/, char* /*argv*/[]) {


    AndroidData::Init();
    return 0;
}



