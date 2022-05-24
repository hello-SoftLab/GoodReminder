#pragma once
#include "../global.h"
#include "ecspp.h"
#include "../android_data/android_data.h"

class Type2D {
private:
    int dummy = 0;
};

class Type3D {
private:
    int dummy = 0;
};


template<typename TextureType>
class Texture {

public:
    Texture(std::function<void(Texture&)> initializerFunc) {
        if (!m_ID) {
            auto deleter = [](unsigned int* id) {
                GL_CALL(glDeleteTextures(1, id));
            };
            m_ID = std::shared_ptr<unsigned int>(new unsigned int, deleter);
            GL_CALL(glGenTextures(1, m_ID.get()));
        }
        FigureOutType();
        this->Bind();
        initializerFunc(*this);
        this->Unbind();
    }

    Texture() {
        FigureOutType();
    }

    operator bool() {
        return m_ID.operator bool();
    }

    GLenum GetType() {
        return m_TextureType;
    }

    void Bind() {
        if (m_ID) {
            GL_CALL(glBindTexture(m_TextureType, *m_ID.get()));
        }

    }
    void Unbind() {
        GL_CALL(glBindTexture(m_TextureType, 0));
    }
    const unsigned int GetID() {
        return *m_ID.get();
    }



private:
    void FigureOutType() {

        if (entt::type_hash<TextureType>() == entt::type_hash<Type2D>()) {
            m_TextureType = GL_TEXTURE_2D;
        }
        if (entt::type_hash<TextureType>() == entt::type_hash<Type3D>()) {
            m_TextureType = GL_TEXTURE_3D;
        }
    };

    GLenum m_TextureType;
    std::shared_ptr<unsigned int> m_ID;


};


void FreeImageData(unsigned char* data);
unsigned char* ReadTextureFile(std::string path,int* width,int* height,int* channels);

template<typename TextureType>
static Texture<TextureType> LoadTextureFromFile(std::string path) {

    if (!std::filesystem::exists(path))
    {
        return {};
    }
    int width, height, nrChannels;


    unsigned char* data = ReadTextureFile(path,&width,&height,&nrChannels);


    GLenum format = nrChannels == 3 ? GL_RGB : GL_RGBA;

    Texture<TextureType> tex([=](Texture<TextureType>& texture){

        texture.Bind();
        GL_CALL(glTexParameteri(texture.GetType(), GL_TEXTURE_WRAP_S, GL_REPEAT));
        GL_CALL(glTexParameteri(texture.GetType(), GL_TEXTURE_WRAP_T, GL_REPEAT));
        GL_CALL(glTexParameteri(texture.GetType(), GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GL_CALL(glTexParameteri(texture.GetType(), GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
        if (ecspp::HelperFunctions::HashClassName<TextureType>() == ecspp::HelperFunctions::HashClassName<Type2D>()) {
            GL_CALL(glTexImage2D(texture.GetType(), 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
        }
        GL_CALL(glGenerateMipmap(texture.GetType()));

        texture.Unbind();
    });

    FreeImageData(data);

    return std::move(tex);

};

