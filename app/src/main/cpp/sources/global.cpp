#include "global.h"
#include "android_data/android_data.h"

bool GetGLError(int line, std::string file) {
    GLenum code = glGetError();
    if (code != GL_NO_ERROR) {
        std::stringstream stream;
        stream << "OpenGL error '" << std::to_string(code) << "' \nAt line: " << line << " \nIn file: " << file << endl;
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,"Error",stream.str().c_str(),AndroidData::CurrentWindow());
        return true;
    }
    return false;
}
