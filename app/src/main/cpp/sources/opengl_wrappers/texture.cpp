#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



void FreeImageData(unsigned char *data) {

    stbi_image_free(data);


}


unsigned char* ReadTextureFile(std::string path, int *width, int *height, int *channels) {
    unsigned char* data = stbi_load(path.c_str(), width, height, channels, 0);
    if (!data) {
        return nullptr;
    }
    return data;


}





