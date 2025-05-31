#pragma once

#include "Assets.h"

class Texture
{
private:
    const Assets::ImageAsset* imageAsset;
    unsigned int rendererID;
    unsigned char* localBuffer;
    int width, height, bitsPerPixel;

public:
    Texture(const Assets::ImageAsset*);
    ~Texture();

    void Bind(unsigned int) const;
    void Unbind() const;
};