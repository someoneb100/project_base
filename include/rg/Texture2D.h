//
// Created by matf-rg on 30.10.20..
//

#ifndef PROJECT_BASE_TEXTURE2D_H
#define PROJECT_BASE_TEXTURE2D_H
#include <glad/glad.h>
#include <stb_image.h>
#include <rg/Error.h>

class Textrure2D{
private:
    unsigned texture_ID;
public:
    Textrure2D(std::string filePath, GLenum texture_wrap, GLenum texture_filter){
        glGenTextures(1, &texture_ID);

        int width, height, nrComponents;
        unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, texture_ID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_wrap);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_wrap);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_filter);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << filePath << std::endl;
            stbi_image_free(data);
        }
    }

    void activate(GLenum textureEnum){
        glActiveTexture(textureEnum);
        glBindTexture(GL_TEXTURE_2D, texture_ID);
    }
};

#endif //PROJECT_BASE_TEXTURE2D_H
