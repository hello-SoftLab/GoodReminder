#include "sources/global.h"
#include "sources/android_data/android_data.h"

extern "C" {

void android_main(struct android_app *app) {

    AndroidData::MainLoop(app);
}

}

