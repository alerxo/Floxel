#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace Assets
{
    struct TextAsset
    {
        const std::string name;
        const std::string text;
    };

    struct ImageAsset
    {
        const std::string name;
        const unsigned int width = 0;
        const unsigned int height = 0;
        // Stored as 4 unsigned bytes as RGBA, 0, 0 is bottom left
        const std::vector<unsigned char> buffer;
    };
    
    extern std::unordered_map<std::string, const TextAsset> textAssets;
    extern std::unordered_map<std::string, const ImageAsset> imageAssets;
}