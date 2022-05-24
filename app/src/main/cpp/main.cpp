#include "sources/global.h"
#include "sources/android_data/android_data.h"
#include "SDL.h"
#include "good_reminder/app_layout.h"
#include "good_reminder/data_structures.h"
#include "android_native_app_glue.h"

static bool firstLoop = true;

int SDL_main(int /*argc*/, char* /*argv*/[]) {



    AppObject obj = AppObject::CreateNew("Hey");

    if(firstLoop) {
        AppLayout::Init();
        firstLoop = false;
    }

    AndroidData::Init();
    return 0;
}

void android_main(struct android_app* app) {
    std::cout << "" << std::endl;

};



extern "C"
JNIEXPORT void JNICALL
Java_knz_goodreminder_MainActivity_LoadImage(JNIEnv *env, jobject thiz, jbyteArray name,
                                             jbyteArray bytes, jint width, jint height) {

    std::string nameStr = "";

    int nameLen = env->GetArrayLength(name);
    jbyte* nameByte = env->GetByteArrayElements(name,nullptr);

    if(!nameByte){
        return;
    }

    for(int i = 0;i<nameLen;i++){
        nameStr += nameByte[i];
    }


    std::vector<unsigned char> data;

    int numBytes = env->GetArrayLength(bytes);

    jbyte* byte = env->GetByteArrayElements(bytes,nullptr);

    if(!byte){
        return;
    }

    for(int i = 0;i<numBytes;i++){
        data.push_back(byte[i]);
    }

    AndroidData::SetImageToBeLoaded(nameStr,data,width,height);



}