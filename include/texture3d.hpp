#ifndef TEXTURE3D_HPP
#define TEXTURE3D_HPP

#include <glad/glad.h>

class Texture3D{
public:
    Texture3D(int size) : _size(size) {
        glGenTextures(1, &_textureID);
        glBindTexture(GL_TEXTURE_3D, _textureID);

            glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, _size, _size, _size, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

            glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
        
        glBindTexture(GL_TEXTURE_3D, 0);
    }

    ~Texture3D() {
        glDeleteTextures(1, &_textureID);
    }

    void BindForWriting(unsigned int unit) const {
        glBindImageTexture(unit, _textureID, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8);
    }

    void GenerateMipMaps() const {
        glBindTexture(GL_TEXTURE_3D, _textureID);
        glGenerateMipmap(GL_TEXTURE_3D);
    }

    void BindForReaeding(unsigned int unit) const {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_3D, _textureID);
    }

private:
    unsigned int _textureID;
    int _size;
};

#endif