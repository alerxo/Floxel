#include "Rendering/Texture.h"

#include "Rendering/Renderer.h"
#include "Debug.h"

Texture::Texture(const Assets::ImageAsset *_imageAsset) : imageAsset(_imageAsset), rendererID(0), localBuffer(nullptr), width(0), height(0)
{
    localBuffer = (unsigned char*)imageAsset->buffer.data();
    width = imageAsset->width;
    height = imageAsset->height;

    GL_CALL(glGenTextures(1, &rendererID));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, rendererID));

    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::~Texture()
{
    GL_CALL(glDeleteTextures(1, &rendererID));
}

void Texture::Bind(unsigned int slot) const
{
    GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, rendererID));
}

void Texture::Unbind() const
{
    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}