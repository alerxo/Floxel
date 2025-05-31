#include "Rendering/Texture.h"

#include "Rendering/Renderer.h"
#include "stb_image.h"
#include "Debug.h"

Texture::Texture(const Assets::ImageAsset *_imageAsset) : imageAsset(_imageAsset), rendererID(0), localBuffer(nullptr), width(0), height(0), bitsPerPixel(0)
{
    stbi_set_flip_vertically_on_load(1);
    localBuffer = stbi_load("test.png", &width, &height, &bitsPerPixel, 4);

    for (int i = 0; i < width * height; i += 4)
    {
        LOG(std::format("Colour ({}), ({}), ({}), ({})", localBuffer[i], localBuffer[i + 1], localBuffer[i + 2], localBuffer[i + 3]));
    }

    GL_CALL(glGenTextures(1, &rendererID));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, rendererID));

    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

    if (localBuffer)
    {
        stbi_image_free(localBuffer);
    }
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